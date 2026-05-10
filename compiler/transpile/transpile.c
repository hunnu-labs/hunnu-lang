#include "transpile.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void transpile_node(ASTNode* node, FILE* out, int indent, int is_expr);

static struct {
    char** names;
    size_t* field_counts;
    char*** field_names;
    size_t count;
    size_t cap;
} type_registry;

static void type_registry_init(void) {
    type_registry.names = NULL;
    type_registry.field_counts = NULL;
    type_registry.field_names = NULL;
    type_registry.count = 0;
    type_registry.cap = 0;
}

static void type_registry_add(const char* name, char** fields, size_t field_count) {
    if (type_registry.count >= type_registry.cap) {
        size_t new_cap = type_registry.cap ? type_registry.cap * 2 : 8;
        type_registry.names = realloc(type_registry.names, sizeof(char*) * new_cap);
        type_registry.field_counts = realloc(type_registry.field_counts, sizeof(size_t) * new_cap);
        type_registry.field_names = realloc(type_registry.field_names, sizeof(char**) * new_cap);
        type_registry.cap = new_cap;
    }
    type_registry.names[type_registry.count] = strdup(name);
    type_registry.field_counts[type_registry.count] = field_count;
    type_registry.field_names[type_registry.count] = malloc(sizeof(char*) * field_count);
    for (size_t i = 0; i < field_count; i++) {
        type_registry.field_names[type_registry.count][i] = strdup(fields[i]);
    }
    type_registry.count++;
}

static char* sanitize_name(const char* name) {
    char* s = strdup(name);
    for (char* p = s; *p; p++) {
        if (*p == '.') *p = '_';
    }
    return s;
}

static void write_indent(FILE* out, int indent) {
    for (int i = 0; i < indent; i++) fprintf(out, "    ");
}

static void write_runtime_header(FILE* out) {
    fprintf(out, "#define _GNU_SOURCE\n");
    fprintf(out, "#include <stdio.h>\n");
    fprintf(out, "#include <stdlib.h>\n");
    fprintf(out, "#include <string.h>\n");
    fprintf(out, "#include <stdint.h>\n\n");
    fprintf(out, "// Hunnu runtime types\n");
    fprintf(out, "typedef enum { HUNNU_INT, HUNNU_FLOAT, HUNNU_STRING, HUNNU_BOOL, HUNNU_NONE, HUNNU_ARRAY, HUNNU_STRUCT } HunnuType;\n");
    fprintf(out, "typedef struct HunnuValue_s { HunnuType type; int64_t i; double f; char* s; int b; struct HunnuValue_s* elems; size_t count; char* struct_type; struct HunnuValue_s* fields; char** field_names; size_t field_count; } HunnuValue;\n\n");
    fprintf(out, "static HunnuValue hunnu_int(int64_t v) { HunnuValue r; r.type = HUNNU_INT; r.i = v; return r; }\n");
    fprintf(out, "static HunnuValue hunnu_float(double v) { HunnuValue r; r.type = HUNNU_FLOAT; r.f = v; return r; }\n");
    fprintf(out, "static HunnuValue hunnu_bool(int v) { HunnuValue r; r.type = HUNNU_BOOL; r.b = v; return r; }\n");
    fprintf(out, "static HunnuValue hunnu_none() { HunnuValue r; r.type = HUNNU_NONE; return r; }\n");
    fprintf(out, "static HunnuValue hunnu_string(const char* v) { HunnuValue r; r.type = HUNNU_STRING; r.s = (v ? strdup(v) : strdup(\"\")); return r; }\n");
    fprintf(out, "static void hunnu_print(HunnuValue v) { switch(v.type) { case HUNNU_INT: printf(\"%%lld\\n\", (long long)v.i); break; case HUNNU_FLOAT: printf(\"%%g\\n\", v.f); break; case HUNNU_STRING: printf(\"%%s\\n\", v.s?v.s:\"\"); break; case HUNNU_BOOL: printf(v.b?\"true\\n\":\"false\\n\"); break; case HUNNU_NONE: printf(\"nil\\n\"); break; case HUNNU_ARRAY: printf(\"[array %%zu items]\\n\", v.count); break; case HUNNU_STRUCT: printf(\"{struct %%s}\\n\", v.struct_type?v.struct_type:\"\"); break; } }\n");
    fprintf(out, "static int hunnu_truthy(HunnuValue v) { switch(v.type) { case HUNNU_BOOL: return v.b; case HUNNU_NONE: return 0; default: return 1; } }\n");
    fprintf(out, "static HunnuValue hunnu_field_get(HunnuValue obj, const char* name) { for (size_t i = 0; i < obj.field_count; i++) { if (strcmp(obj.field_names[i], name) == 0) return obj.fields[i]; } return hunnu_none(); }\n\n");
}

static void transpile_literal(ASTNode* node, FILE* out) {
    switch (node->data.literal.literal_type) {
        case TOKEN_INT_LITERAL:
            fprintf(out, "hunnu_int(%lld)", (long long)node->data.literal.value.int_value);
            break;
        case TOKEN_FLOAT_LITERAL:
            fprintf(out, "hunnu_float(%g)", node->data.literal.value.float_value);
            break;
        case TOKEN_STRING_LITERAL: {
            char* s = node->data.literal.value.string_value;
            fprintf(out, "hunnu_string(");
            if (s) {
                fprintf(out, "\"");
                for (char* p = s; *p; p++) {
                    if (*p == '\\') fprintf(out, "\\\\");
                    else if (*p == '"') fprintf(out, "\\\"");
                    else if (*p == '\n') fprintf(out, "\\n");
                    else if (*p == '\t') fprintf(out, "\\t");
                    else fputc(*p, out);
                }
                fprintf(out, "\"");
            } else {
                fprintf(out, "\"\"");
            }
            fprintf(out, ")");
            break;
        }
        case TOKEN_TRUE:
            fprintf(out, "hunnu_bool(1)");
            break;
        case TOKEN_FALSE:
            fprintf(out, "hunnu_bool(0)");
            break;
        case TOKEN_NULL: case TOKEN_NIL_KEYWORD:
            fprintf(out, "hunnu_none()");
            break;
        default:
            fprintf(out, "hunnu_none()");
            break;
    }
}

static void transpile_call_expr(ASTNode* node, FILE* out) {
    char* sfname = sanitize_name(node->data.call_expr.name);
    fprintf(out, "hunnu_call_%s(", sfname);
    free(sfname);
    for (size_t i = 0; i < node->data.call_expr.arg_count; i++) {
        if (i > 0) fprintf(out, ", ");
        transpile_node(node->data.call_expr.args[i], out, 0, 1);
    }
    fprintf(out, ")");
}

static void transpile_binary_expr(ASTNode* node, FILE* out) {
    ASTNode* left = node->data.binary_expr.left;
    ASTNode* right = node->data.binary_expr.right;

    int is_cmp = 0;
    const char* op_str = "";
    switch (node->data.binary_expr.operator) {
        case TOKEN_PLUS: op_str = "+"; break;
        case TOKEN_MINUS: op_str = "-"; break;
        case TOKEN_STAR: op_str = "*"; break;
        case TOKEN_SLASH: op_str = "/"; break;
        case TOKEN_PERCENT: op_str = "%%"; break;
        case TOKEN_EQ: op_str = "=="; is_cmp = 1; break;
        case TOKEN_NEQ: op_str = "!="; is_cmp = 1; break;
        case TOKEN_LT: op_str = "<"; is_cmp = 1; break;
        case TOKEN_LE: op_str = "<="; is_cmp = 1; break;
        case TOKEN_GT: op_str = ">"; is_cmp = 1; break;
        case TOKEN_GE: op_str = ">="; is_cmp = 1; break;
        default: op_str = "+"; break;
    }

    if (is_cmp) {
        fprintf(out, "({ HunnuValue _l = ");
        transpile_node(left, out, 0, 1);
        fprintf(out, "; HunnuValue _r = ");
        transpile_node(right, out, 0, 1);
        fprintf(out, "; hunnu_bool(_l.i %s _r.i); })", op_str);
    } else {
        fprintf(out, "({ HunnuValue _l = ");
        transpile_node(left, out, 0, 1);
        fprintf(out, "; HunnuValue _r = ");
        transpile_node(right, out, 0, 1);
        fprintf(out, "; HunnuValue _res; _res.type = HUNNU_INT; _res.i = _l.i %s _r.i; _res; })", op_str);
    }
}

static void transpile_unary_expr(ASTNode* node, FILE* out) {
    if (node->data.unary_expr.operator == TOKEN_MINUS) {
        fprintf(out, "({ HunnuValue _v = ");
        transpile_node(node->data.unary_expr.operand, out, 0, 1);
        fprintf(out, "; HunnuValue _r; _r.type = HUNNU_INT; _r.i = -_v.i; _r; })");
    } else if (node->data.unary_expr.operator == TOKEN_NOT) {
        fprintf(out, "({ HunnuValue _v = ");
        transpile_node(node->data.unary_expr.operand, out, 0, 1);
        fprintf(out, "; hunnu_bool(!hunnu_truthy(_v)); })");
    } else {
        transpile_node(node->data.unary_expr.operand, out, 0, 1);
    }
}

static void transpile_array_expr(ASTNode* node, FILE* out) {
    fprintf(out, "({ HunnuValue _arr; _arr.type = HUNNU_ARRAY; _arr.count = %zu; _arr.elems = malloc(sizeof(HunnuValue)*_arr.count); ", node->data.array_expr.count);
    for (size_t i = 0; i < node->data.array_expr.count; i++) {
        fprintf(out, "_arr.elems[%zu] = ", i);
        transpile_node(node->data.array_expr.elements[i], out, 0, 1);
        fprintf(out, "; ");
    }
    fprintf(out, "_arr; })");
}

static void transpile_index_expr(ASTNode* node, FILE* out) {
    fprintf(out, "({ HunnuValue _arr = ");
    transpile_node(node->data.index_expr.array, out, 0, 1);
    fprintf(out, "; HunnuValue _idx = ");
    transpile_node(node->data.index_expr.index, out, 0, 1);
    fprintf(out, "; _arr.elems[_idx.i]; })");
}

static void transpile_field_access(ASTNode* node, FILE* out) {
    fprintf(out, "hunnu_field_get(");
    transpile_node(node->data.field_access.object, out, 0, 1);
    fprintf(out, ", \"%s\")", node->data.field_access.field);
}

static void transpile_struct_instance(ASTNode* node, FILE* out) {
    const char* tn = node->data.struct_instance.type_name;
    fprintf(out, "({ HunnuValue _s; _s.type = HUNNU_STRUCT; _s.struct_type = strdup(\"%s\"); _s.field_count = %zu; _s.fields = malloc(sizeof(HunnuValue) * %zu); _s.field_names = malloc(sizeof(char*) * %zu); ", tn, node->data.struct_instance.field_count, node->data.struct_instance.field_count, node->data.struct_instance.field_count);
    for (size_t i = 0; i < node->data.struct_instance.field_count; i++) {
        fprintf(out, "_s.fields[%zu] = ", i);
        transpile_node(node->data.struct_instance.field_values[i], out, 0, 1);
        fprintf(out, "; _s.field_names[%zu] = strdup(\"%s\"); ", i, node->data.struct_instance.field_names[i]);
    }
    fprintf(out, "_s; })");
}

static void transpile_method_call(ASTNode* node, FILE* out) {
    char* smethod = sanitize_name(node->data.method_call.method);
    int is_static = 0;
    char* type_name = NULL;
    if (node->data.method_call.object->type == AST_IDENTIFIER) {
        type_name = node->data.method_call.object->data.identifier.name;
        for (size_t i = 0; i < type_registry.count; i++) {
            if (strcmp(type_registry.names[i], type_name) == 0) {
                is_static = 1;
                break;
            }
        }
    }
    if (is_static) {
        char* full_name = malloc(strlen(type_name) + 1 + strlen(smethod) + 1);
        sprintf(full_name, "%s_%s", type_name, smethod);
        fprintf(out, "hunnu_call_%s(", full_name);
        free(full_name);
        for (size_t i = 0; i < node->data.method_call.arg_count; i++) {
            if (i > 0) fprintf(out, ", ");
            transpile_node(node->data.method_call.args[i], out, 0, 1);
        }
        fprintf(out, ")");
    } else {
        fprintf(out, "({ fprintf(stderr, \"Runtime error: instance method calls not supported in AOT\\n\"); exit(1); hunnu_none(); })");
    }
    free(smethod);
}

static void transpile_node(ASTNode* node, FILE* out, int indent, int is_expr) {
    if (!node) return;

    switch (node->type) {
        case AST_LITERAL:
            transpile_literal(node, out);
            break;

        case AST_IDENTIFIER: {
            char* name = node->data.identifier.name;
            if (strcmp(name, "true") == 0) { fprintf(out, "hunnu_bool(1)"); break; }
            if (strcmp(name, "false") == 0) { fprintf(out, "hunnu_bool(0)"); break; }
            if (strcmp(name, "null") == 0 || strcmp(name, "nil") == 0) { fprintf(out, "hunnu_none()"); break; }
            fprintf(out, "_var_%s", name);
            break;
        }

        case AST_CALL_EXPR:
            transpile_call_expr(node, out);
            break;

        case AST_BINARY_EXPR:
            transpile_binary_expr(node, out);
            break;

        case AST_UNARY_EXPR:
            transpile_unary_expr(node, out);
            break;

        case AST_ARRAY_EXPR:
            transpile_array_expr(node, out);
            break;

        case AST_INDEX_EXPR:
            transpile_index_expr(node, out);
            break;

        case AST_FIELD_ACCESS:
            transpile_field_access(node, out);
            break;

        case AST_STRUCT_INSTANCE:
            transpile_struct_instance(node, out);
            break;

        case AST_METHOD_CALL:
            transpile_method_call(node, out);
            break;

        case AST_ASSIGN:
            write_indent(out, indent);
            fprintf(out, "_var_%s = ", node->data.assign.name);
            transpile_node(node->data.assign.value, out, 0, 1);
            fprintf(out, ";\n");
            break;

        case AST_VAR_DECL:
            write_indent(out, indent);
            fprintf(out, "HunnuValue _var_%s = ", node->data.var_decl.name);
            if (node->data.var_decl.initializer) {
                transpile_node(node->data.var_decl.initializer, out, 0, 1);
            } else {
                fprintf(out, "hunnu_none()");
            }
            fprintf(out, ";\n");
            break;

        case AST_FN_DECL: {
            char* fname = node->data.fn_decl.name;
            char* sfname = sanitize_name(fname);
            int is_main = (strcmp(fname, "main") == 0);

            if (is_main) {
                fprintf(out, "\nint main(int argc, char** argv) {\n");
            } else {
                fprintf(out, "\nHunnuValue hunnu_call_%s(", sfname);
                for (size_t i = 0; i < node->data.fn_decl.param_count; i++) {
                    if (i > 0) fprintf(out, ", ");
                    fprintf(out, "HunnuValue _var_%s", node->data.fn_decl.params[i]);
                }
                fprintf(out, ") {\n");
            }

            if (node->data.fn_decl.body) {
                transpile_node(node->data.fn_decl.body, out, 1, 0);
            }

            if (is_main) {
                fprintf(out, "    return 0;\n");
            } else {
                fprintf(out, "    return hunnu_none();\n");
            }
            fprintf(out, "}\n");
            free(sfname);
            break;
        }

        case AST_BLOCK:
            for (size_t i = 0; i < node->data.block.count; i++) {
                transpile_node(node->data.block.statements[i], out, indent, 0);
            }
            break;

        case AST_PRINT_STMT:
            write_indent(out, indent);
            fprintf(out, "hunnu_print(");
            transpile_node(node->data.print_stmt.argument, out, 0, 1);
            fprintf(out, ");\n");
            break;

        case AST_RETURN_STMT:
            write_indent(out, indent);
            fprintf(out, "return ");
            if (node->data.return_stmt.value) {
                transpile_node(node->data.return_stmt.value, out, 0, 1);
            } else {
                fprintf(out, "hunnu_none()");
            }
            fprintf(out, ";\n");
            break;

        case AST_EXPR_STMT:
            write_indent(out, indent);
            transpile_node(node->data.expr_stmt.expression, out, 0, 1);
            fprintf(out, ";\n");
            break;

        case AST_IF_STMT:
            write_indent(out, indent);
            fprintf(out, "if (hunnu_truthy(");
            transpile_node(node->data.if_stmt.condition, out, 0, 1);
            fprintf(out, ")) {\n");
            transpile_node(node->data.if_stmt.then_branch, out, indent + 1, 0);
            if (node->data.if_stmt.else_branch) {
                write_indent(out, indent);
                fprintf(out, "} else {\n");
                transpile_node(node->data.if_stmt.else_branch, out, indent + 1, 0);
            }
            write_indent(out, indent);
            fprintf(out, "}\n");
            break;

        case AST_WHILE_STMT:
            write_indent(out, indent);
            fprintf(out, "while (hunnu_truthy(");
            transpile_node(node->data.while_stmt.condition, out, 0, 1);
            fprintf(out, ")) {\n");
            transpile_node(node->data.while_stmt.body, out, indent + 1, 0);
            write_indent(out, indent);
            fprintf(out, "}\n");
            break;

        case AST_FOR_STMT: {
            write_indent(out, indent);
            fprintf(out, "for (HunnuValue _for_var = ");
            if (node->data.for_stmt.initializer) {
                transpile_node(node->data.for_stmt.initializer, out, 0, 1);
            } else {
                fprintf(out, "hunnu_none()");
            }
            fprintf(out, "; hunnu_truthy(");
            if (node->data.for_stmt.condition) {
                transpile_node(node->data.for_stmt.condition, out, 0, 1);
            } else {
                fprintf(out, "hunnu_bool(1)");
            }
            fprintf(out, "); ");
            if (node->data.for_stmt.update) {
                transpile_node(node->data.for_stmt.update, out, 0, 1);
            } else {
                fprintf(out, "hunnu_none()");
            }
            fprintf(out, ") {\n");
            transpile_node(node->data.for_stmt.body, out, indent + 1, 0);
            write_indent(out, indent);
            fprintf(out, "}\n");
            break;
        }

        case AST_TYPE_DECL:
            break;

        case AST_EXTERN_FN: {
            char* fname = node->data.extern_fn.name;
            char* sfname = sanitize_name(fname);
            fprintf(out, "\nextern HunnuValue hunnu_call_%s(", sfname);
            free(sfname);
            for (size_t i = 0; i < node->data.extern_fn.param_count; i++) {
                if (i > 0) fprintf(out, ", ");
                fprintf(out, "HunnuValue _var_%s", node->data.extern_fn.param_names[i]);
            }
            fprintf(out, ");\n");
            break;
        }

        case AST_ADDRESS_OF:
        case AST_DEREFERENCE:
        case AST_STRING_CONCAT:
        case AST_MATCH_EXPR:
        case AST_TRY_STMT:
        case AST_INDEX_ASSIGN:
        case AST_CLASS_DECL:
        case AST_NEW_EXPR:
        case AST_FIELD_ASSIGN:
            write_indent(out, indent);
            fprintf(out, "// Unsupported: %s\n", ast_node_type_to_string(node->type));
            break;

        case AST_BREAK_STMT:
            write_indent(out, indent);
            fprintf(out, "break;\n");
            break;

        case AST_CONTINUE_STMT:
            write_indent(out, indent);
            fprintf(out, "continue;\n");
            break;

        default:
            break;
    }
}

char* transpile_to_c(ASTNode* program) {
    char* buffer = malloc(1024 * 1024);
    if (!buffer) return NULL;
    buffer[0] = '\0';

    FILE* out = fmemopen(buffer, 1024 * 1024, "w");
    if (!out) { free(buffer); return NULL; }

    type_registry_init();

    write_runtime_header(out);

    if (program->type == AST_PROGRAM) {
        for (size_t i = 0; i < program->data.program.count; i++) {
            ASTNode* stmt = program->data.program.statements[i];
            if (stmt->type == AST_TYPE_DECL) {
                type_registry_add(stmt->data.type_decl.name, stmt->data.type_decl.fields, stmt->data.type_decl.field_count);
                    (void)stmt->data.type_decl.is_pub;
            } else if (stmt->type == AST_CLASS_DECL) {
                type_registry_add(stmt->data.class_decl.name, stmt->data.class_decl.fields, stmt->data.class_decl.field_count);
                    (void)stmt->data.class_decl.is_pub;
            }
        }
        for (size_t i = 0; i < program->data.program.count; i++) {
            transpile_node(program->data.program.statements[i], out, 0, 0);
        }
    }

    fclose(out);
    return buffer;
}
