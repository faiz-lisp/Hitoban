/*
* Code by Folaefolc
* A Lisp-like done just to concurrence Lisp itself (kind of crazy game for me)
* Interpreted programming language, C++14 ; main purpose is for video games
* License MIT
*/

#pragma once

#include "htb_types.hpp"
#include "htb_includes.hpp"
#include "htb_internal.hpp"

namespace htb
{

    constexpr long INF_NB_ARGS      = -1;
    constexpr long AT_LEAST_1_ARGS  = -2;
    constexpr long AT_LEAST_2_ARGS  = -3;
    constexpr long BETWEEN_0_1_ARGS = -4;
    constexpr long BETWEEN_0_2_ARGS = -5;

    ///////////////////////////////////////////////////// cell

    struct environment; // forward declaration; cell and environment reference each other
    struct cell;

    typedef cell (*proc_type)(const std::vector<cell>&);
    typedef std::vector<cell> cells;
    typedef std::map<std::string, cell> cell_dict;
    typedef cells::const_iterator cellit;
    typedef cell return_type;
    typedef const cells& args_type;

    // a variant that can hold any kind of Hitoban value
    struct cell
    {
        typedef std::vector<cell>::const_iterator iter;

        // htb value holder #optimization
        struct value {
            std::tuple<std::string, long, cells, cell_dict, proc_type> holder;
            long number_of_args;

            value();
            value(const value& v);
            void init_from(value* v);

            template <typename T> T get()             {}
            template <typename T> T get()       const {}
            template <typename T> T& get_ref()        {}
            template <typename T> void set(const T)   {}

            bool operator==(const value& other) const;
        };

        value val;
        cell_type type;
        environment* env;
        bool const_expr;

        cell(cell_type type=Symbol);
        template <typename T> cell(cell_type type, T) :type(type), env(0), const_expr(false) {}

        cell(proc_type proc, long n=0);
        cell(const cell& c);
        void init_from(cell* c);
        cell exec(const cells& c, const std::string& name);
        cell get_in(const std::string& key);
        cell get_in(long n);

        bool operator==(const cell& r) const;
        bool operator!=(const cell& r) const;
    };  // struct cell

    // template specialization (non-const versions)
    template <> std::string cell::value::get<std::string>();
    template <> long        cell::value::get<long       >();
    template <> cells       cell::value::get<cells      >();
    template <> cell_dict   cell::value::get<cell_dict  >();
    template <> proc_type   cell::value::get<proc_type  >();
    // const version
    template <> std::string cell::value::get<std::string>() const;
    template <> long        cell::value::get<long       >() const;
    template <> cells       cell::value::get<cells      >() const;
    template <> cell_dict   cell::value::get<cell_dict  >() const;
    template <> proc_type   cell::value::get<proc_type  >() const;
    // references versions
    template <> std::string& cell::value::get_ref<std::string>();
    template <> long&        cell::value::get_ref<long       >();
    template <> cells&       cell::value::get_ref<cells      >();
    template <> cell_dict&   cell::value::get_ref<cell_dict  >();
    template <> proc_type&   cell::value::get_ref<proc_type  >();
    // setters
    template <> void         cell::value::set<std::string>(std::string v);
    template <> void         cell::value::set<long       >(long v);
    template <> void         cell::value::set<cells      >(cells v);
    template <> void         cell::value::set<cell_dict  >(cell_dict v);
    template <> void         cell::value::set<proc_type  >(proc_type v);
    // cells initializers
    template <> cell::cell<std::string>(cell_type type, std::string v);
    template <> cell::cell<long       >(cell_type type, long v);
    template <> cell::cell<cells      >(cell_type type, cells v);
    template <> cell::cell<cell_dict  >(cell_type type, cell_dict v);
    template <> cell::cell<proc_type  >(cell_type type, proc_type v);

    const cell false_sym(Symbol, "false");
    const cell true_sym(Symbol, "true"); // anything that isn't false_sym is true_sym
    const cell nil(Symbol, "nil");

    ///////////////////////////////////////////////////// environment

    // a dictionary that (a) associates symbols with cells, and
    // (b) can chain to an "outer" dictionary
    struct environment {
        bool isfile;
        std::string fname;

        environment(environment* outer=0);
        environment(const cells& parms, const cells& args, environment* outer);
        // return a reference to the innermost environment where 'var' appears
        cell_dict& find(const std::string& var);
        // return a reference to the cell associated with the given symbol 'var'
        cell& operator[] (const std::string & var);
        bool has_outer();
        // get a namespace or create one
        environment* get_namespace(const std::string& name);
        // get a namespace or return 0 if it doesn't exist
        environment* _get_namespace(const std::string& name);
        // search for an environment which is a file, and return its path
        std::string get_parent_file();
        // return a list of string (names of the available namespaces)
        std::list<std::string> get_namespaces();

    private:
        environment* outer_; // next adjacent outer env, or 0 if there are no further environments
        cell_dict env_; // inner symbol->cell mapping
        cell_dict errors;
        std::map<std::string, environment*> namespaces;
    };  // struct environment

} // namespace htb
