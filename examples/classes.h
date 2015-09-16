/*
 * classes.h
 *
 *      Author: Dominik Sadko
 */

#ifndef CLASSES_H_
#define CLASSES_H_

class ClassA
{
	public:
		ClassA(const std::string& param) : m_param(param) {};
		~ClassA() {};

		void setParam(const std::string& param) { m_param = param; }
		std::string getParam() const { return m_param; }
		static std::string staticParam() { return "null"; }

	private:
		std::string m_param;
};

void LuaBinder_Classes()
{
    g_lua.newMetatable("ClassA");
    g_lua.constructor<ClassA, const std::string&>("ClassA.new");
    g_lua.destructor<ClassA>("ClassA.delete");
    g_lua.sharedFunction(&ClassA::setParam, "ClassA.setParam");
    g_lua.sharedFunction(&ClassA::getParam, "ClassA.getParam");
    g_lua.staticFunction(&ClassA::staticParam, "ClassA.staticParam");


    g_lua.callString("local C = ClassA.new('test') \n"
    				 "assert(C:getParam() == 'test') \n"
    				 "C:setParam('works') \n"
			 	 	 "assert(C:getParam() == 'works') \n"
    				 "C:delete() \n"
    				 "assert(ClassA.staticParam() == 'null') \n"
    );

    ClassA classA = ClassA("Instance");
    g_lua.instanceFunction(&ClassA::getParam, &classA, "ClassAInstancedGetParam");
    g_lua.callString("assert(ClassAInstancedGetParam() == 'Instance') \n");
}


#endif /* CLASSES_H_ */
