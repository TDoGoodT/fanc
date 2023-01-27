
#ifndef ANALYZER_VALIDATIONS_HPP
#define ANALYZER_VALIDATIONS_HPP

#include "symbol_table_manager.hpp"
#include "actions.hpp"

namespace validations {
    bool is_call_correct(Store *store, struct Call *call, int lineno);

    bool is_id_declared(Store *store, struct Id *id);

    bool is_func_declared(Store *store, struct Id *func_id);

    bool is_type_compatible(struct Exp *exp1, struct Exp *exp2);

    bool has_value(struct Exp *exp);

    bool is_bool(struct Exp *exp);

    bool is_castable(Type::TypeCase fromType, Type::TypeCase toTypeCase, bool explicit_cast = false);

    bool is_byte_to_large(struct Byte *byte);

    bool is_formal_declared(vector<struct FormalDecl *> &formal, Id *id);

    void validate_main(Store *pStore);


};
#endif //ANALYZER_VALIDATIONS_HPP