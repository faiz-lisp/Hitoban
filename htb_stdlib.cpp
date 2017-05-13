#include "htb_stdlib.hpp"

namespace htb
{

cell proc_add(const cells& c)
{
    long n = to_long(c[0].val);
    for (cellit i = c.begin()+1; i != c.end(); ++i)
        n += to_long(i->val);

    return cell(Number, str(n));
}

cell proc_sub(const cells& c)
{
    long n = to_long(c[0].val);
    for (cellit i = c.begin()+1; i != c.end(); ++i)
    {
        if (i->type != Number)
        {
            std::stringstream ss; ss << "Not a number : " << i->val << std::endl;
            return cell(Exception, ss.str());
        }
        n -= to_long(i->val);
    }

    return cell(Number, str(n));
}

cell proc_mul(const cells& c)
{
    long n(1);
    for (cellit i = c.begin(); i != c.end(); ++i)
    {
        if (i->type != Number)
        {
            std::stringstream ss; ss << "Not a number : " << i->val << std::endl;
            return cell(Exception, ss.str());
        }
        n *= to_long(i->val);
    }

    return cell(Number, str(n));
}

cell proc_div(const cells& c)
{
    long n = to_long(c[0].val);
    for (cellit i = c.begin()+1; i != c.end(); ++i)
    {
        if (i->type != Number)
        {
            std::stringstream ss; ss << "Not a number : " << i->val << std::endl;
            return cell(Exception, ss.str());
        }
        n /= to_long(i->val);
    }

    return cell(Number, str(n));
}

cell proc_greater(const cells& c)
{
    long n = to_long(c[0].val);
    for (cellit i = c.begin()+1; i != c.end(); ++i)
    {
        if (i->type != Number)
        {
            std::stringstream ss; ss << "Not a number : " << i->val << std::endl;
            return cell(Exception, ss.str());
        }
        if (n <= to_long(i->val))
            return false_sym;
    }
    return true_sym;
}

cell proc_less(const cells& c)
{
    long n = to_long(c[0].val);
    for (cellit i = c.begin()+1; i != c.end(); ++i)
    {
        if (i->type != Number)
        {
            std::stringstream ss; ss << "Not a number : " << i->val << std::endl;
            return cell(Exception, ss.str());
        }
        if (n >= to_long(i->val))
            return false_sym;
    }
    return true_sym;
}

cell proc_less_equal(const cells& c)
{
    long n = to_long(c[0].val);
    for (cellit i = c.begin()+1; i != c.end(); ++i)
    {
        if (i->type != Number)
        {
            std::stringstream ss; ss << "Not a number : " << i->val << std::endl;
            return cell(Exception, ss.str());
        }
        if (n > to_long(i->val))
            return false_sym;
    }
    return true_sym;
}

cell proc_eq(const cells& c)
{
    for (cellit i = c.begin()+1; i != c.end(); ++i)
    {
        if (i->type != Number)
        {
            std::stringstream ss; ss << "Not a number : " << i->val << std::endl;
            return cell(Exception, ss.str());
        }
        if (c[0].val != i->val)
            return false_sym;
    }
    return true_sym;
}

cell proc_length(const cells& c)
{
    return cell(Number, str(c[0].list.size()));
}

cell proc_nullp(const cells& c)
{
    return c[0].list.empty() ? true_sym : false_sym;
}

cell proc_car(const cells& c)
{
    if (!c[0].list.empty())
        return c[0].list[0];

    std::stringstream ss; ss << "List empty" << std::endl;
    return cell(Exception, ss.str());
}

cell proc_cdr(const cells& c)
{
    if (c[0].list.size() < 2)
        return nil;

    cell result(c[0]);
    result.list.erase(result.list.begin());

    return result;
}

cell proc_append(const cells& c)
{
    if (c.size() < 2)
    {
        std::stringstream ss; ss << "Missing argument(s) for append" << std::endl;
        return cell(Exception, ss.str());
    }

    cell result(List);
    result.list = c[0].list;
    for (cellit i = c[1].list.begin(); i != c[1].list.end(); ++i)
        result.list.push_back(*i);

    return result;
}

cell proc_cons(const cells& c)
{
    if (c.size() < 2)
    {
        std::stringstream ss; ss << "Missing argument(s) for cons" << std::endl;
        return cell(Exception, ss.str());
    }

    cell result(List);
    result.list.push_back(c[0]);
    for (cellit i = c[1].list.begin(); i != c[1].list.end(); ++i)
        result.list.push_back(*i);

    return result;
}

cell proc_list(const cells& c)
{
    cell result(List);
    result.list = c;

    return result;
}

cell proc_nth(const cells& c)
{
    if (c.size() < 2)
    {
        std::stringstream ss; ss << "Missing argument(s) for nth" << std::endl;
        return cell(Exception, ss.str());
    }

    if (c[1].type == List)
    {
        long n = to_long(c[0].val);
        cell tmp = c[1];
        const cell temp = tmp.get_in(n);
        cell result;
        if (temp.type == List || temp.type == Dict)
        {
            result.type = temp.type;

            if (result.type == List)
                result.list = temp.list;
            else if (result.type == Dict)
                result.dict = temp.dict;
        }
        else
        {
            result.type = temp.type;
            result.val = temp.val;
        }

        return result;
    }
    else if (c[1].type == Dict)
    {
        cell tmp = c[1];
        const cell temp = tmp.get_in(c[0].val);
        cell result;
        if (temp.type == List || temp.type == Dict)
        {
            result.type = temp.type;

            if (result.type == List)
                result.list = temp.list;
            else if (result.type == Dict)
                result.dict = temp.dict;
        }
        else
        {
            result.type = temp.type;
            result.val = temp.val;
        }

        return result;
    }
    else
    {
        std::stringstream ss; ss << "The object should be a list or a dict, not of type " << convert_htbtype(c[1].type) << std::endl;
        return cell(Exception, ss.str());
    }
}

cell proc_dict(const cells& c)
{
    cell result(Dict);

    cell::map m;
    for (cellit i = c[1].list.begin(); i != c[1].list.end(); ++i)
    {
        if ((*i).type != List)
        {
            std::stringstream ss; ss << "Arguments of dict should be list" << std::endl;
            return cell(Exception, ss.str());
        }
        else if ((*i).list[0].type != String)
        {
            std::stringstream ss; ss << "Keys in dict should only be strings" << std::endl;
            return cell(Exception, ss.str());
        }
        else
        {
            std::string key((*i).list[0].val);
            if ((*i).list.size() > 2)  // we have more than 2 elements, not normal
            {
                std::stringstream ss; ss << "Tuples to define (key value) in dict should not be of size " << (*i).list.size() << std::endl;
                return cell(Exception, ss.str());
            }
            m[key.substr(1, key.size() - 2)] = (*i).list[1];
        }
    }

    result.dict = m;

    return result;
}

///////////////////////////////////////////////////// built-in functions
std::map<std::string, cell> get_builtin()
{
    std::map<std::string, cell> builtin;
    /* list */
    builtin["append"] = cell(&proc_append);
    builtin["car"] = cell(&proc_car);
    builtin["cdr"]  = cell(&proc_cdr);
    builtin["cons"] = cell(&proc_cons);
    builtin["length"] = cell(&proc_length);
    builtin["list"] = cell(&proc_list);
    builtin["null?"] = cell(&proc_nullp);
    builtin["nth"] = cell(&proc_nth);
    /* operators */
    builtin["+"] = cell(&proc_add);
    builtin["-"] = cell(&proc_sub);
    builtin["*"] = cell(&proc_mul);
    builtin["/"] = cell(&proc_div);
    /* conditionals operators */
    builtin[">"]  = cell(&proc_greater);
    builtin["<"] = cell(&proc_less);
    builtin["<="] = cell(&proc_less_equal);
    builtin["="] = cell(&proc_eq);
    /* dictionary */
    builtin["dict"] = cell(&proc_dict);

    return builtin;
}

} // namespace htb
