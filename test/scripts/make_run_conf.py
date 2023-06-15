import jinja2
import os
import sys
jinja_template = """
<component name="ProjectRunConfigurationManager">
  <configuration default="false" name="fanc {{name}}" type="CMakeRunConfiguration" factoryName="Application" REDIRECT_INPUT="true" REDIRECT_INPUT_PATH="$PROJECT_DIR$/test/compiler/input/{{name}}" ELEVATE="false" USE_EXTERNAL_CONSOLE="false" PASS_PARENT_ENVS_2="true" PROJECT_NAME="fanc_analyzer" TARGET_NAME="fanc" CONFIG_NAME="Debug" RUN_TARGET_PROJECT_NAME="fanc_analyzer" RUN_TARGET_NAME="fanc">
    <method v="2">
      <option name="com.jetbrains.cidr.execution.CidrBuildBeforeRunTaskProvider$BuildBeforeRunTask" enabled="true" />
    </method>
  </configuration>
</component>
"""
path = os.path.dirname(os.path.realpath(__file__))
parent_path = os.path.abspath(os.path.join(path, os.pardir))
project_root = os.path.abspath(os.path.join(parent_path, os.pardir))
input_dir = os.path.join(parent_path, 'compiler/input')
for file in os.listdir(input_dir):
    print(file)
    if file.endswith('.in'):
        jinja2.Template(jinja_template).stream(name=file).dump(os.path.join(project_root, ".run", file[:-3] + '.xml'))