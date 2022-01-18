"""Set of general utility functions to be used in layer generation."""

import os
import subprocess
import jinja2
from third_party.vkspecgen import vkapi


def _get_script_dir() -> str:
    """Returns the directory of this python script."""
    return os.path.dirname(os.path.realpath(__file__))


def get_vk_xml_path() -> str:
    """Returns the path to vk.xml in yeti/third_party/Vulkan-Headers/registry/vk.xml."""
    return 'third_party/Vulkan-Headers/registry/vk.xml'


def get_common_templates_dir() -> str:
    """Returns the directory containing the template files."""
    return os.path.join(_get_script_dir(), 'templates')


def generate_file(name: str, output_dir: str, env: jinja2.Environment,
                  parameters) -> None:
    """Generates an output file from a jinja template.

    Args:
      name: the name of the file to be generated.
      output_dir: where to write the generated file.
      env: VkSpecGen-compliant Jinja2 environment.
      parameters: set of parameters to be passed to the template rendering engine.
    """
    output_path = os.path.join(output_dir, name)
    tmp = env.get_template(name + '.jinja2')
    tmp.stream(parameters).dump(output_path)
    if output_path.endswith('.go'):
        subprocess.call(['gofmt', '-w', output_path])
    elif (output_path.endswith('.h') or output_path.endswith('.cpp') or
          output_path.endswith('.cc')):
        subprocess.call(['clang-format', '-i', '-style=file', output_path])


def generate_manifest_file(manifest_name: str, output_dir: str,
                           env: jinja2.Environment, parameters) -> None:
    """Generates an output file from a jinja template.

    Args:
      manifest_name: the name of the manifest file to be generated.
      output_dir: where to write the generated file.
      env: VkSpecGen-compliant Jinja2 environment.
      parameters: set of parameters to be passed to the template rendering engine.
    """
    output_path = os.path.join(output_dir, manifest_name)
    tmp = env.get_template('manifest.json.jinja2')
    tmp.stream(parameters).dump(output_path)


def visit_type(t: vkapi.TypeModifier, visited: dict[str, vkapi.TypeModifier]):
    """Generates a dictionary of types used to define a given type.

    Args:
      t: the name of the type to recursively go through its members or base types.
      visited: dictionary of types used to define the given type.
    """
    if t.name not in visited:
        if isinstance(t, vkapi.Struct):
            for p in t.members:
                visit_type(p.type, visited)
            visited[t.name] = t
        elif isinstance(t, vkapi.Pointer) or isinstance(
                t, vkapi.DynamicArray) or isinstance(t, vkapi.FixedArray):
            visit_type(t.base_type, visited)
        else:
            visited[t.name] = t


def referenced_types(commands: list[str]):
    """Returns the set of types referenced by a command list.

    Args:
        commands: list of commands to investigate.
    """
    visited = {}
    for cmd in commands:
        for p in cmd.parameters:
            visit_type(p.type, visited)

    return visited
