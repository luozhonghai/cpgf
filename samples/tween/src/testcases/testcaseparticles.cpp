#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4996)
#endif

#include "testcase.h"

#include "sprite.h"
#include "easeinfo.h"

#include "cpgf/tween/gtweenlist.h"
#include "cpgf/accessor/gaccessor.h"

#include "wx/log.h"

#if defined(_WIN32)
    #include <windows.h>
#endif
#include <GL/gl.h>

#include <vector>

using namespace std;
using namespace cpgf;

class TestCaseParticles : public TestCase
{
public:
	TestCaseParticles();

	virtual void render(int viewWidth, int viewHeight);
	virtual void setEase(int easeIndex);
	virtual void reset();

private:
	void setRatio(float ratio);

private:
	vector<float> fromTo; // fromX, fromY, toX, toY
	vector<float> particles; // startX, startY, endX, endY
};

const int particleCountInRow = 100;
const int particleCountInColumn = 100;
const float xOffset = 0.001f;
const float yOffset = 0.001f;
const int duration = 2000;

TestCasePtr createTestCaseParticles()
{
	return TestCasePtr(new TestCaseParticles);
}

TestCaseParticles::TestCaseParticles()
{
	this->reset();
}

void TestCaseParticles::render(int viewWidth, int viewHeight)
{
	size_t count = this->particles.size();
	if(count == 0) {
		return;
	}

    glDisable(GL_LIGHTING);

	glColor3f(0.8f, 0.8f, 0.8f);
	glVertexPointer(2, GL_FLOAT, 0, &this->particles[0]);
	glDrawArrays(GL_LINES, 0, count / 2);
}

void TestCaseParticles::setRatio(float ratio)
{
	size_t count = this->particles.size();
	for(size_t i = 0; i < count; i += 4) {
		float x = this->fromTo[i] + (this->fromTo[i + 2] - this->fromTo[i]) * ratio;
		float y = this->fromTo[i + 1] + (this->fromTo[i + 3] - this->fromTo[i + 1]) * ratio;
		this->particles[i] = x;
		this->particles[i + 1] = y;
		this->particles[i + 2] = x + xOffset;
		this->particles[i + 3] = y + yOffset;
	}
}

void TestCaseParticles::setEase(int easeIndex)
{
	this->reset();

	GTweenEaseType ease = getEase(easeIndex)->ease;
	GTween & tween = GTweenList::getInstance()->to(duration)
		.ease(ease)
		.tween(createAccessor(this, 0, &TestCaseParticles::setRatio), 0.0f, 1.0f)
	;
}

void TestCaseParticles::reset()
{
	GTweenList::getInstance()->clear();

	if(this->particles.size() == 0) {
		float spaceX = 2.0f / (particleCountInRow + 1);
		float spaceY = 2.0f / (particleCountInColumn + 1);

		for(int c = 0; c < particleCountInColumn; ++c) {
			for(int r = 0; r < particleCountInRow; ++r) {
				float x = spaceX * (r + 1);
				float y = spaceY * (c + 1);
				
				this->fromTo.push_back(x);
				this->fromTo.push_back(y);
				this->fromTo.push_back(1.0f);
				this->fromTo.push_back(1.0f);

				this->particles.push_back(x);
				this->particles.push_back(y);
				this->particles.push_back(x + xOffset);
				this->particles.push_back(y + yOffset);
			}
		}
	}
}

