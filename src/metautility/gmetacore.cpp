#include "cpgf/metautility/gmetacore.h"
#include "cpgf/gmetaapi.h"
#include "cpgf/scriptbind/gscriptbind.h"


namespace cpgf {

GMetaCore::GMetaCore(GScriptObject * scriptObject)
	: scriptObject(scriptObject)
{
}

IMetaClass * GMetaCore::cloneClass(IMetaClass * metaClass)
{
	return this->scriptObject->cloneMetaClass(metaClass);
}

GVariant GMetaCore::cast(const GVariant & instance, IMetaClass * targetMetaClass)
{
	GVariant value = getVariantRealValue(instance);
	GMetaType type = getVariantRealMetaType(instance);
	
	if(canFromVariant<void *>(value) && type.getBaseName() != NULL) {
		GScopedInterface<IMetaClass> sourceClass(this->scriptObject->getMetaService()->findClassByName(type.getBaseName()));
		if(sourceClass) {
			void * ptr = objectAddressFromVariant(instance);
			void * oldPtr = ptr;
			if(ptr != NULL) {
				GMetaType targetType;
				if(targetMetaClass != NULL) {
					ptr = metaCastToDerived(oldPtr, sourceClass.get(), targetMetaClass);
					if(ptr == NULL) {
						ptr = metaCastToBase(oldPtr, sourceClass.get(), targetMetaClass);
					}
					targetType = metaGetItemType(targetMetaClass);
				}
				else {
					GScopedInterface<IMetaClass> derivedClass(findAppropriateDerivedClass(oldPtr, sourceClass.get(), &ptr));
					if(derivedClass) {
						targetType = metaGetItemType(derivedClass.get());
					}
					else {
						ptr = NULL;
					}
				}

				if(ptr != NULL) {
					targetType.addPointer();
					return createTypedVariant(pointerToObjectVariant(ptr), targetType);
				}
			}
		}
	}

	return (void *)0;
}


} // namespace cpgf
