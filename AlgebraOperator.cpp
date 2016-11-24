#include "AlgebraOperator.hpp"

#include <set>
#include <algorithm>

namespace AlgebraOperator
{

        bool Attribute::operator<(Attribute const& rhs) const
        {
            if(this->table_name == rhs.table_name)
                return (this->attribute_name < rhs.attribute_name);
            return (this->table_name < rhs.table_name);
        }
        
        bool Attribute::operator==(Attribute const& rhs) const
        {
            return (this->table_name == rhs.table_name) && (this->attribute_name == rhs.attribute_name);
        }
        
        
        void HashJoin::setParent(std::shared_ptr<AlgebraOperator> sp) 
        {
            std::cout << "setParent: hashjoin" << std::endl;
            this->parent = sp;
            this->left->setParent(shared_from_this());
            this->right->setParent(shared_from_this());
        }
        
        
        std::vector<Attribute> HashJoin::producesAttr() 
        {
            std::set<Attribute> produced;
        
            //get produced attributes
            auto left_tmp = left->producesAttr();
            auto right_tmp = right->producesAttr();
            
            //if left child is join, then only left side
            if(std::dynamic_pointer_cast<HashJoin>(left) == nullptr)
            {
                std::for_each(left_tmp.begin(), left_tmp.end(), [&](Attribute attr){ produced.insert(attr);});
            
            
                std::for_each(right_tmp.begin(), right_tmp.end(), [&](Attribute const& attr){ produced.insert(attr);});
            }
            else
            {
                    std::for_each(left_tmp.begin(), left_tmp.end(), [&](Attribute attr){ produced.insert(attr);});
            
            }
            std::cout << "Table name1 = " << this->tablename_left << "; table name2 = " << this->tablename_right << std::endl;
            std::cout << "AlgebraOperator.cpp line 62: no parent   Table1 ptr = " << this->left << "; table name2 = " << this->right << std::endl;
            std::set<Attribute> required;
            //get attributes that parents require
            if(auto sp_parent = this->parent.lock())
            {
                auto tmp = sp_parent->requires();
                std::for_each(tmp.begin(), tmp.end(), [&](Attribute const& attr){required.insert(attr);});   
            }
            else
            {
                std::cout << "\n\nERRORRRRRRRR print weakptr" << "\n" << std::endl;         
            }
            
            std::vector<Attribute> intersection;

            std::set_intersection(produced.begin(), produced.end(), required.begin(), required.end(), std::back_inserter(intersection));
            
        
            return intersection;
        }
        
        std::vector<Attribute> HashJoin::requires() 
        {
            if(auto sp_parent = this->parent.lock())
                return sp_parent->requires();
            throw std::bad_exception();
        }
        
        void HashJoin::produce(std::shared_ptr<AlgebraOperator> parent, std::stringstream& ss) 
        {
            this->parent = parent;
            
            //TODO: Key Compare combine_hash() as templated Functor 
            ss << "std::unordered_multimap<std::tuple<";
            //types
            for(auto i = 0; i < join_attributes.size(); i++)
            {
                auto tmp_attr = join_attributes[i].first;
                ss << tmp_attr.type;
                if(i < join_attributes.size()-1)
                    ss << ", ";
            }
            //std::cout << "Integer, Integer, Integer";
            
            //end types
            ss << ">, ";
            
            //change to tuple<....> <- specifiy which attributes are needed
            ss << "std::tuple<";
            
            std::vector<Attribute> value_attributes;
            if(auto sp_parent = this->parent.lock())
                value_attributes = sp_parent->requires();
            else
                throw std::bad_exception();
            /*
            * WATCH OUT PARENT MUST BE SET BEFORE
            * 
            */
            auto input_attr_l = this->left->producesAttr();
        // auto input_attr_r = this->right->producesAttr();
    
            //remove attributes that arent produced by input
            value_attributes.erase(std::remove_if(value_attributes.begin(), value_attributes.end(), [&](Attribute const& attr){ 
                auto left_search = std::find(input_attr_l.begin(), input_attr_l.end(), attr);            
            //   auto right_search = std::find(input_attr_r.begin(), input_attr_r.end(), attr);
    
                return (left_search == input_attr_l.end());// && (right_search == input_attr_r.end());
            }), value_attributes.end());
            
            for(auto i = 0; i < value_attributes.size(); i++)
            {
                ss << " " << value_attributes[i].type;
                if(i < value_attributes.size() - 1)
                    ss << ", ";
            }
            
            // Insert RESULT Types 
            ss << ">";
            
            ss << ", ";
            for(auto i = 0; i < join_attributes.size(); i++)
                ss << "Int";
            //ss << "IntIntInt";
            ss << "Hash> ";//TODO combine_hash from num Int
            
            ss << "join_";
            ss << this->join_attributes[0].first.table_name << "_";
            ss << this->join_attributes[0].second.table_name << "_HT;" << std::endl;
            left->produce(shared_from_this(), ss);
            right->produce(shared_from_this(), ss);
        
        }
        
        
        
        
        
        std::vector<Attribute> HashJoin::addToValueOfHT(std::shared_ptr<HashJoin> curr)
        {
            std::vector<Attribute> value_attributes;
            if(auto sp_parent = curr->parent.lock())
                value_attributes = sp_parent->requires();
            else
                throw std::bad_exception();
            /*
            * WATCH OUT PARENT MUST BE SET BEFORE
            * 
            */
            auto input_attr_l = curr->left->producesAttr();
            //auto input_attr_r = this->right->producesAttr();
        
            //remove attributes that arent produced by input
            value_attributes.erase(std::remove_if(value_attributes.begin(), value_attributes.end(), [&](Attribute const& attr){ 
                auto left_search = std::find(input_attr_l.begin(), input_attr_l.end(), attr);            
                //auto right_search = std::find(input_attr_r.begin(), input_attr_r.end(), attr);
        
                return (left_search == input_attr_l.end());// && (right_search == input_attr_r.end());
            }), value_attributes.end());
    
            return value_attributes;
    
        }
        
        
        
        
        void HashJoin::consume(std::shared_ptr<AlgebraOperator> curr, std::stringstream& ss) 
        {
            //only for left
            if(curr == left)
            {
                //if(std::dynamic_pointer_cast<HashJoin>(left) == nullptr)
                {
                    ss << "join_";
                    ss << this->join_attributes[0].first.table_name << "_";
                    ss << this->join_attributes[0].second.table_name << "_HT";
                    ss << ".emplace(std::make_tuple(";
                    for(auto i = 0; i < this->join_attributes.size(); i++)
                    {
                        ss << "db.m_" << this->join_attributes[i].first.table_name << ".";
                        ss << this->join_attributes[i].first.attribute_name << "()[tid]";
                        if(i < this->join_attributes.size()-1)
                            ss << ", ";
                    }
                    ss << "), ";
                if(std::dynamic_pointer_cast<HashJoin>(left) != nullptr)
                {
                    ss << "std::tuple_cat(it->second, ";
                }
                    ss << "std::make_tuple(";
                    //TODO
        //----------------------------------------------------------
        // get attributes that will be generated by this operator
        //----------------------------------------------------------
        
            std::vector<Attribute> new_value_attributes = addToValueOfHT(shared_from_this());
                    
            auto childHash = std::dynamic_pointer_cast<HashJoin>(left);
            if(childHash != nullptr)
            {
                //remove attributes that have been added previously to only add new attrs.
                std::vector<Attribute> new_value_attributes_old = addToValueOfHT(childHash);
                
                //get two sets
                std::set<Attribute> prev_produced;
                    
                    std::for_each(new_value_attributes_old.begin(), new_value_attributes_old.end(), [&](Attribute attr){ prev_produced.insert(attr);});
                
                    std::set<Attribute> now_produced;
                    //get attributes that parents require
                    if(auto sp_parent = this->parent.lock())
                    {
                        std::for_each(new_value_attributes.begin(), new_value_attributes.end(), [&](Attribute const& attr){now_produced.insert(attr);});   
                    }
                //difference 
                std::vector<Attribute> diff;
                
                std::set_difference(now_produced.begin(), now_produced.end(), prev_produced.begin(), prev_produced.end(), std::back_inserter(diff));
                //remove all that are not element of difference               
                
                new_value_attributes.erase(std::remove_if(new_value_attributes.begin(), new_value_attributes.end(), [&](Attribute const& attr){ 
                    auto left_search = std::find(diff.begin(), diff.end(), attr);            
            
        
                    return (left_search == diff.end());// && (right_search == input_attr_r.end());
                }), new_value_attributes.end());
                
                
            }
                    
            for(auto i = 0; i < new_value_attributes.size(); i++)
            {
                ss << "db.m_" << new_value_attributes[i].table_name << "." << new_value_attributes[i].attribute_name << "()[tid]";
                if(i < new_value_attributes.size() - 1)
                    ss << ", ";
            }
            //----------------------------------------------------------
                    ss << "))";
                    if(std::dynamic_pointer_cast<HashJoin>(left) != nullptr)
                        ss << ")" << std::endl;
                    ss << ";" << std::endl;
                }
            }
            else
            {
                ss <<"auto range = ";
                ss << "join_";
                ss << this->join_attributes[0].first.table_name << "_";
                ss << this->join_attributes[0].second.table_name << "_HT";
                
                ss << ".equal_range(std::make_tuple(";
                
                for(auto i = 0; i < this->join_attributes.size(); i++)
                {
                    ss << "db.m_" << this->join_attributes[i].second.table_name << ".";
                    ss << this->join_attributes[i].second.attribute_name << "()[tid]";
                    if(i < this->join_attributes.size()-1)
                        ss << ", ";
                }
                
                
                ss << "));" << std::endl;
                ss << "for(auto it = range.first; it != range.second; ++it)\n{\n" << std::endl;
                
                
                if(auto sp_parent = this->parent.lock())
                {
                    //if(std::dynamic_pointer_cast<Print>(sp_parent) == nullptr)
                    //       ss << "store t_tmp and t_b in HT_b" << std::endl;
                    
                    sp_parent->consume(shared_from_this(), ss);                
                }
                ss << "}\n" << std::endl; 
            }
        }

        bool HashJoin::isHashJoinAbove()
        {
            return true;
        }

        
        void TableScan::setParent(std::shared_ptr<AlgebraOperator> sp) 
        {
            std::cout << "setParent: tablescan" << std::endl;
            this->parent = sp;
        }
        
        std::vector<Attribute> TableScan::producesAttr() 
        {
            //all attributes of this table
            return this->attributes;
        }
        
        std::vector<Attribute> TableScan::requires() 
        {
            //always leaf -> should never be called
            throw std::bad_exception();
        }
        
        void TableScan::produce(std::shared_ptr<AlgebraOperator> parent, std::stringstream& ss) 
        {
            this->parent = parent;
            ss << "for(auto tid = 0; tid < db.m_";
            ss << this->table_name << ".size(); tid++)\n{" << std::endl;
            if(auto sp_parent = this->parent.lock())
                sp_parent->consume(shared_from_this(), ss);
            ss << "}\n" << std::endl;
        }
        void TableScan::consume(std::shared_ptr<AlgebraOperator> curr, std::stringstream& ss) 
        {
            //Nothing to do here
        }
        
        bool TableScan::isHashJoinAbove()
        {
            if(auto s_ptr = this->parent.lock())
            {
                return s_ptr->isHashJoinAbove();
            }
            throw;
        }

        
        //Selection
        
        void Selection::setParent(std::shared_ptr<AlgebraOperator> sp) 
        {
            std::cout << "setParent: selection" << std::endl;
            this->parent = sp;
            input->setParent(shared_from_this());
        }
        
        
        std::vector<Attribute> Selection::producesAttr() 
        {
            return input->producesAttr();
        }
        
        std::vector<Attribute> Selection::requires() 
        {
            if(auto sp_parent = this->parent.lock())
                return sp_parent->requires();
            else
                throw std::bad_exception();
        }
        
        void Selection::produce(std::shared_ptr<AlgebraOperator> parent, std::stringstream& ss) 
        {
            this->parent = parent;
            input->produce(shared_from_this(), ss);
        }
        void Selection::consume(std::shared_ptr<AlgebraOperator> curr, std::stringstream& ss) 
        {
            ss << "\tif(";
            
            for(int i = 0; i < this->clauses.size(); i++)
            {
                    //first always attrobute-name
                    ss << "db.m_" << this->clauses[i].table_name << "." << this->clauses[i].attribute << "()[tid]";
                    ss << " == ";
                    if(this->clauses[i].isConstant)
                        ss << this->clauses[i].value;
                    else
                        ss << "db.m_" << this->clauses[i].table_name2 << "." << this->clauses[i].value << "()[tid]";
                    
                    if(i < this->clauses.size()-1)
                        ss << " && ";
            }
            
            ss << ")\n\t{" << std::endl;
            if(auto sp_parent = parent.lock())
                sp_parent->consume(shared_from_this(), ss);
            ss << "\n\t}" << std::endl;
        }
        
        bool Selection::isHashJoinAbove()
        {
            if(auto s_ptr = this->parent.lock())
            {
                return s_ptr->isHashJoinAbove();
            }
            throw;
        }
        
        /*
         * Print
         */
        void Print::setParent(std::shared_ptr<AlgebraOperator> sp) 
        {
            std::cout << "setParent: print" << std::endl;
            this->parent = sp;
            input->setParent(shared_from_this());
        }
        
        //dummy wont ever get called, root always topmost
        std::vector<Attribute> Print::producesAttr() 
        {
            return std::vector<Attribute>();
        }
        
        std::vector<Attribute> Print::requires() 
        {
            return this->attributes;
        }
        
        
        void Print::produce(std::shared_ptr<AlgebraOperator> parent, std::stringstream& ss) 
        {
            parent = parent;
            input->produce(shared_from_this(), ss);
        };
        void Print::consume(std::shared_ptr<AlgebraOperator> curr, std::stringstream& ss) 
        {
            ss << "\t\tstd::cout";
            
            auto produced_by_child = input->producesAttr();
            if(this->isHashJoinAbove())
            {
                for(auto i = 0; i < produced_by_child.size(); i++)
                {
                    ss << " << std::get<" << i << ">(it->second) << \" \" ";
                    
                }
                
                for(auto i = produced_by_child.size(); i < this->attributes.size(); i++)
                {
                    ss << " << db.m_" << this->attributes[i].table_name << "." << this->attributes[i].attribute_name << "()[" << "tid" << "] << \" \"";            
                }
            }
            else
            {
                //No HashJoin above so just print attributes directly
                auto produced_by_child = input->producesAttr();
                for(auto i = 0; i < this->attributes.size(); i++)
                {
                    ss << " << db.m_" << this->attributes[i].table_name << "." << this->attributes[i].attribute_name << "()[" << "tid" << "] << \" \"";            
                }
            }
            ss << " << std::endl;" << std::endl;
        }
        
        bool Print::isHashJoinAbove()
        {
            return false;
        }

}
