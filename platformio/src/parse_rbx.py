# ================================================================================
# This python script reads the class info in "rbx.h" and automatically generates 
# duktape function wrappers in "duktape_rbx.cpp".
# ================================================================================
#
# parse_rbx.py
#
# Copyright 2020 Victor Chew
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import re

def parse_func_defs(funcparamtext):
  funcparamtextlist = [x.strip() for x in funcparamtext.split(',')]
  funcparamlist = []
  for funcparamtext in funcparamtextlist:
    if len(funcparamtext) > 0: 
      tokens = [x.strip() for x in re.split("[=]+", funcparamtext)]
      ltokens = [x.strip() for x in re.split("\s+", tokens[0])]
      ptype = " ".join(ltokens[0:len(ltokens)-1])
      pname = ltokens[len(ltokens)-1]
      if len(tokens) == 1: funcparamlist.append({ "type": ptype, "name": pname, "required": True})
      else: funcparamlist.append({ "type": ptype, "name": pname, "defval": tokens[1], "required": False})
  return funcparamlist

def get_func_param_str(params):
  result = []
  for param in params:
    if param["required"]: result.append(param["name"])
    else: result.append(param["name"] + "=" + param["defval"])
  return ", ".join(result) 

def get_func_param_cstr(params):
  result = []
  for param in params: result.append(param["name"])
  return ", ".join(result) 

def required_params_count(params):
  return sum(p["required"] == True for p in params)

def handle_params_validation(name, params):
  if len(params) > 0:
    print("\tif (duk_get_top(ctx) < {}) {{".format(required_params_count(params)), file=out_cpp)
    prototype = name + "(" + get_func_param_str(params) + ")"
    print("\t\tduk_push_error_object(ctx, DUK_ERR_TYPE_ERROR, \"{}\");".format(prototype), file=out_cpp)
    print("\t\treturn duk_throw(ctx);", file=out_cpp)
    print("\t}", file=out_cpp)

def handle_params_retrieval(params):
  for i, param in enumerate(params):
    if param["required"]:
      if param["type"] == "int" or param["type"] == "float":
        print("\t{} {} = ({})duk_require_number(ctx, {});".format(param["type"], param["name"], param["type"], i), file=out_cpp)
      if param["type"] == "bool":
        print("\t{} {} = ({})duk_require_boolean(ctx, {});".format(param["type"], param["name"], param["type"], i), file=out_cpp)
      elif param['type'] == 'const char*':
        print("\t{} {} = ({})duk_require_string(ctx, {});".format(param["type"], param["name"], param["type"], i), file=out_cpp)
    else:
      if param["type"] == "int" or param["type"] == "float":
        print("\t{} {} = ({})duk_get_number_default(ctx, {}, {});".format(param["type"], param["name"], param["type"], i, param["defval"]), file=out_cpp)
      elif param["type"] == "bool":
        print("\t{} {} = ({})duk_get_boolean_default(ctx, {}, {});".format(param["type"], param["name"], param["type"], i, param["defval"]), file=out_cpp)
      elif param['type'] == 'const char*':
        print("\t{} {} = ({})duk_get_string_default(ctx, {}, {});".format(param["type"], param["name"], param["type"], i, param["defval"]), file=out_cpp)

infile = open("lib/rbx/rbx.h")
header = infile.read()
infile.close()

out_cpp = open("src/duktape_rbx.cpp", "w")

# Parse header
re_class = re.compile(r"class\s+([^;}]+?)\s+\{\s+public\:(.+?)private\:(.+?)};", re.DOTALL)
re_constructor = re.compile(r"([^ ]+?)\s*\((.*?)\);", re.DOTALL)
re_function = re.compile(r"(.+?)\s+(.+?)\s*\((.*?)\)[;| |{]", re.DOTALL)
classlist = []
for match in re_class.finditer(header):
  classname = match.group(1)
  index = classname.find(':')
  if index >= 0: classname = classname[0:index-1]
  if classname == 'Keystore': continue
  print(classname)
  classfunctext = match.group(2)
  classfunctextlist = [x.strip() for x in classfunctext.split('\n')]
  classfunctextlist = list(filter(lambda x: not x.startswith("~" + classname), classfunctextlist)) # Ignore destructors
  classfunctextlist = list(filter(lambda x: not x.startswith("virtual ~" + classname), classfunctextlist)) # Ignore virtual destructors
  classfunctextlist = list(filter(lambda x: not x.startswith("// "), classfunctextlist)) # Ignore comments
  classfunclist = []
  for classfunctext in classfunctextlist:
    match = re_function.match(classfunctext)
    if match:
      functype = match.group(1)
      funcname = match.group(2)
      funcparamtext = match.group(3)
      funcparamlist = parse_func_defs(match.group(3))
      if not funcname.startswith("_"): classfunclist.append({ "type": functype, "name" : funcname, "params": funcparamlist })
    else:
      match = re_constructor.match(classfunctext)
      if match:
        funcname = match.group(1)
        if funcname == classname:
          funcparamlist = parse_func_defs(match.group(2))
          classfunclist.append({ "type": "new", "name" : funcname, "params": funcparamlist })
  classlist.append({ "name": classname, "methods": classfunclist })

# Preamable
print("/*", file=out_cpp)
print(" * THIS FILE IS GENERATED BY <parse_rbx.py>. DO NOT EDIT THIS FILE DIRECTLY.", file=out_cpp)
print(" */\n", file=out_cpp)

# Headers
print("#include <rbx.h>", file=out_cpp)
print("#include <duktape.h>", file=out_cpp)
print("#include <webserver.h>\n", file=out_cpp)

# Constant enums for RBX classes
class_enums = []
for rbxclass in classlist: class_enums.append(rbxclass["name"].upper())
print("enum {{ {} }};\n".format(", ".join(class_enums)), file=out_cpp)

print("int rbx_ptr_count = 0;", file=out_cpp)
print("int rbx_type[64];", file=out_cpp)
print("void* rbx_ptr[64];\n", file=out_cpp)

print("void add_rbx_ptr(int type, void* ptr) {", file=out_cpp)
print("\trbx_type[rbx_ptr_count] = type;", file=out_cpp)
print("\trbx_ptr[rbx_ptr_count] = ptr;", file=out_cpp)
print("\t++rbx_ptr_count;", file=out_cpp)
print("}\n", file=out_cpp)

# Create matching Duktape C functions
for rbxclass in classlist:
  # Class methods 
  for func in rbxclass["methods"]:
    # Constructor
    if func["type"] == "new":
      comment = "// " + rbxclass["name"] + "(" + get_func_param_str(func["params"]) + ")"
      print(comment, file=out_cpp)
      print("duk_ret_t {}_constructor(duk_context* ctx) {{".format(rbxclass["name"]), file=out_cpp)
      print("\tif (!duk_is_constructor_call(ctx)) return DUK_RET_TYPE_ERROR;", file=out_cpp)
      handle_params_validation(func["name"], func["params"])
      handle_params_retrieval(func["params"])
      print("\t{} *ptr = new {}({});".format(rbxclass["name"], rbxclass["name"], get_func_param_cstr(func["params"])), file=out_cpp)
      print("\tadd_rbx_ptr({}, ptr);".format(rbxclass["name"].upper()), file=out_cpp)
      print("\tduk_push_object(ctx);", file=out_cpp)
      print("\tduk_get_global_string(ctx, \"{}_prototype\");".format(rbxclass["name"]), file=out_cpp)
      print("\tduk_set_prototype(ctx, -2);", file=out_cpp)    
      print("\tduk_push_pointer(ctx, (void*)ptr);", file=out_cpp)
      print("\tduk_put_prop_string(ctx, -2, DUK_HIDDEN_SYMBOL(\"_ptr\"));", file=out_cpp)
      print("\treturn 1;", file=out_cpp)
      print("}\n", file=out_cpp)
    # Other methods
    else:
      comment = "// " + func["type"] + " " + rbxclass["name"] + "." + func["name"] + "(" + get_func_param_str(func["params"]) + ")"
      print(comment, file=out_cpp)
      print("duk_ret_t {}_{}(duk_context* ctx) {{".format(rbxclass["name"], func["name"]), file=out_cpp)
      handle_params_validation(func["name"], func["params"])
      handle_params_retrieval(func["params"])
      print("\tduk_push_this(ctx);", file=out_cpp)
      print("\tduk_get_prop_string(ctx, -1, DUK_HIDDEN_SYMBOL(\"_ptr\"));", file=out_cpp)
      print("\t{}* ptr = ({}*)duk_get_pointer_default(ctx, -1, NULL);".format(rbxclass["name"], rbxclass["name"]), file=out_cpp)
      if func["type"] == "void": 
        print("\tif (ptr) ptr->{}({});".format(func["name"], get_func_param_cstr(func["params"])), file=out_cpp)
        print("\treturn 0;", file=out_cpp)
      elif func["type"] == "bool":
        print("\tif (ptr) {", file=out_cpp)
        print("\t\tbool result = ptr->{}({});".format(func["name"], get_func_param_cstr(func["params"])), file=out_cpp)
        print("\t\tduk_push_boolean(ctx, result);", file=out_cpp)
        print("\t\treturn 1;", file=out_cpp)
        print("\t}", file=out_cpp)
        print("\treturn 0;", file=out_cpp)
      elif func["type"] == "int":
        print("\tif (ptr) {", file=out_cpp)
        print("\t\tint result = ptr->{}({});".format(func["name"], get_func_param_cstr(func["params"])), file=out_cpp)
        print("\t\tduk_push_number(ctx, result);", file=out_cpp)
        print("\t\treturn 1;", file=out_cpp)
        print("\t}", file=out_cpp)
        print("\treturn 0;", file=out_cpp)
      elif func["type"] == "float":
        print("\tif (ptr) {", file=out_cpp)
        print("\t\tfloat result = ptr->{}({});".format(func["name"], get_func_param_cstr(func["params"])), file=out_cpp)
        print("\t\tduk_push_number(ctx, result);", file=out_cpp)
        print("\t\treturn 1;", file=out_cpp)
        print("\t}", file=out_cpp)
        print("\treturn 0;", file=out_cpp)
      elif func["type"] == "static": continue
      else: print("Unknown return type '{}' for function: {}".format(func["type"], func["name"]))
      print("}\n", file=out_cpp)
  # Class initializer
  print("void {}_init(duk_context* ctx) {{".format(rbxclass["name"]), file=out_cpp)
  print("\tduk_push_object(ctx);", file=out_cpp)
  for func in rbxclass["methods"]:
    if func["type"] != "new":
      numargs = "DUK_VARARGS"
      if len(func["params"]) == 0: numargs = 0
      print("\tduk_push_c_function(ctx, {}_{}, {});".format(rbxclass["name"], func["name"], numargs), file=out_cpp)
      print("\tduk_put_prop_string(ctx, -2, \"{}\");".format(func["name"]), file=out_cpp)
  print("\tduk_put_global_string(ctx, \"{}_prototype\");".format(rbxclass["name"]), file=out_cpp)
  print("\tduk_push_c_function(ctx, {}_constructor, DUK_VARARGS);".format(rbxclass["name"]), file=out_cpp)
  print("\tduk_put_global_string(ctx, \"{}\");".format(rbxclass["name"]), file=out_cpp)  
  print("}\n", file=out_cpp)

# Master init
print("void RBX_init(duk_context* ctx) {", file=out_cpp)

# Delete instances created previously
print("\t// Delete instances created previously", file=out_cpp)
print("\tfor (int i=0; i<rbx_ptr_count; i++) {", file=out_cpp)
print("\t\tswitch(rbx_type[i]) {", file=out_cpp)
for rbxclass in classlist:
  print("\t\t\tcase {}: delete ({}*)rbx_ptr[i]; break;".format(rbxclass["name"].upper(), rbxclass["name"]), file=out_cpp)
print("\t\t}", file=out_cpp)
print("\t}", file=out_cpp)
print("\trbx_ptr_count = 0;", file=out_cpp)

#Enums
re_enum = re.compile("enum\s+(.+?)\s+\{(.+?)\};")
for match in re_enum.finditer(header):
  name = match.group(1)
  text = match.group(2)
  print("\t// {} constants".format(name), file=out_cpp)
  constants = [x.strip() for x in text.split(',')]
  for index, constant in enumerate(constants):
    print("\tduk_push_number(ctx, {}); duk_put_global_string(ctx, \"{}\");".format(index, constant), file=out_cpp)

# All classes
print("\t// RBX classes", file=out_cpp)
for rbxclass in classlist:
  print("\t{}_init(ctx);".format(rbxclass["name"]), file=out_cpp)

print("}\n", file=out_cpp)
