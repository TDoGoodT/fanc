import jinja2
import os
import sys
jinja_template = """
<component name="ProjectRunConfigurationManager">
  <configuration default="false" name="{{name}}" type="CMakeRunConfiguration" factoryName="Application" REDIRECT_INPUT="true" REDIRECT_INPUT_PATH="$PROJECT_DIR$/test/input/{{name}}" ELEVATE="false" USE_EXTERNAL_CONSOLE="false" PASS_PARENT_ENVS_2="true" PROJECT_NAME="analyzer" TARGET_NAME="analyzer" CONFIG_NAME="Debug" RUN_TARGET_PROJECT_NAME="analyzer" RUN_TARGET_NAME="analyzer">
    <method v="2">
      <option name="RunConfigurationTask" enabled="true" run_configuration_name="Analyzer" run_configuration_type="MAKEFILE_TARGET_RUN_CONFIGURATION" />
      <option name="com.jetbrains.cidr.execution.CidrBuildBeforeRunTaskProvider$BuildBeforeRunTask" enabled="true" />
    </method>
  </configuration>
</component>
"""
path = os.path.dirname(os.path.realpath(__file__))
parent_path = os.path.abspath(os.path.join(path, os.pardir))
input_dir = os.path.join(parent_path, 'input')
for file in os.listdir(input_dir):
    print(file)
    if file.endswith('.in'):
        jinja2.Template(jinja_template).stream(name=file).dump(os.path.join(input_dir, file[:-3] + '.xml'))