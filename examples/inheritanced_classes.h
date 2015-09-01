/*
 * shared_classes.h
 *
 *  Created on: 25 sie 2015
 *      Author: Dominik Sadko
 */

#ifndef INHERITANCED_CLASSES_H_
#define INHERITANCED_CLASSES_H_

class ClassB : public ClassA
{
	public:
		ClassB(const std::string& id, const std::string& param) : ClassA(param), m_id(id) { };
		~ClassB() {};

		std::string getId() const { return m_id; }

	private:
		std::string m_id;
};

void LuaBinder_InheritancedClasses()
{
    g_lua.newMetatable("ClassB", "ClassA");
    g_lua.constructor<ClassB, const std::string&, const std::string&>("ClassB.new");
    g_lua.destructor<ClassB>("ClassB.delete");
    g_lua.sharedFunction(&ClassB::getId, "ClassB.getId");

    g_lua.callString("local C = ClassB.new('id_0x01', 'testX') \n"
    				 "assert(C:getParam() == 'testX') \n"
    				 "C:setParam('worksX') \n"
			 	 	 "assert(C:getParam() == 'worksX') \n"
    				 "assert(C:getId() == 'id_0x01') \n"
    				 "C:delete() \n"
    );
}

#endif /* INHERITANCED_CLASSES_H_ */
