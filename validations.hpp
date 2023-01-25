
#ifndef ANALYZER_VALIDATIONS_HPP
#define ANALYZER_VALIDATIONS_HPP

#include "symbol_table_manager.hpp"
#include "actions.hpp"

namespace validations {
    bool is_call_correct(Store *store, struct _T_Call *call, int lineno);

    bool is_id_declared(Store *store, struct _T_Id *id);

    bool is_func_declared(Store *store, struct _T_Id *func_id);

    bool is_type_compatible(struct _T_Exp *exp1, struct _T_Exp *exp2);

    bool has_value(struct _T_Exp *exp);

    bool is_bool(struct _T_Exp *exp);

    bool is_castable(_T_Type::Type fromType, _T_Type::Type toTypeCase, bool explicit_cast = false);

    bool is_byte_to_large(struct _T_Byte *byte);

    bool is_formal_declared(vector<struct _T_FormalDecl *> &formal, _T_Id *id);
};
#endif //ANALYZER_VALIDATIONS_HPP