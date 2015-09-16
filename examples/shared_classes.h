/*
 * object_class.h
 *
 *      Author: Dominik Sadko
 */

#ifndef SHARED_CLASSES_H_
#define SHARED_CLASSES_H_

class ClassC : public Lua::Shared
{
	public:
		ClassC() { setMetatable("ClassC"); };
		virtual ~ClassC() {};
};

void LuaBinder_SharedClasses()
{
	g_lua.newMetatable("ClassC");
	g_lua.constructor<ClassC>("ClassC.new");
    g_lua.destructor<ClassC>("ClassC.delete");
    g_lua.staticFunction(&Lua::Object::isRemoved, "ClassC.isRemoved");
    g_lua.sharedFunction(&ClassC::setMetatable, "ClassC.setMetatable");
    g_lua.sharedFunction(&ClassC::getMetatable, "ClassC.getMetatable");

    g_lua.callString("local C = ClassC.new() \n"

    				 "C.test = 'test' \n"					// We can set field by use Lua::Shared
			 	 	 "assert(C.test == 'test') \n"
    				 "assert(ClassC.test == nil) \n"

    				 "assert(C:getMetatable() == 'ClassC') \n"

    				 "assert(C:isRemoved() == false) \n"
			 	 	 "C:delete() \n"
    				 "assert(C:isRemoved() == true) \n"
    );
}

#endif /* OBJECT_CLASSES_H_ */
