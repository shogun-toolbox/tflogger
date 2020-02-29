import os
import re
import sys
import platform
import subprocess

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
from distutils.version import LooseVersion
from distutils import dir_util, file_util
from distutils.command.sdist import sdist


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=''):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)

class (sdist):
    def __init__():

    self.template =
class CMakeBuild(build_ext):
    def run(self):
        try:
            out = subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError("CMake must be installed to build the following extensions: " +
                               ", ".join(e.name for e in self.extensions))

        if platform.system() == "Windows":
            cmake_version = LooseVersion(re.search(r'version\s*([\d.]+)', out.decode()).group(1))
            if cmake_version < '3.12.0':
                raise RuntimeError("CMake >= 3.12.0 is required on Windows")

        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        extdir = os.path.abspath(os.path.join(os.path.dirname(self.get_ext_fullpath(ext.name)), 'tflogger'))
        cmake_args = ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + extdir,
                      '-DPython_ROOT_DIR=' + sys.exec_prefix,
                      '-DINTERFACE_PYTHON=ON']

        cfg = 'Debug' if self.debug else 'Release'
        build_args = ['--config', cfg]

        if platform.system() == "Windows":
            cmake_args += ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{}={}'.format(cfg.upper(), extdir)]
            if sys.maxsize > 2**32:
                cmake_args += ['-A', 'x64']
            build_args += ['--', '/m']
        else:
            cmake_args += ['-DCMAKE_BUILD_TYPE=' + cfg]
            build_args += ['--', '-j2']

        env = os.environ.copy()
        env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format(env.get('CXXFLAGS', ''),
                                                              self.distribution.get_version())
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)
        subprocess.check_call(['cmake', ext.sourcedir] + cmake_args, cwd=self.build_temp, env=env)
        subprocess.check_call(['cmake', '--build', '.'] + build_args, cwd=self.build_temp)
        dir_util.copy_tree(os.path.join(self.build_temp, 'python', 'tflogger'), extdir)
        file_util.copy_file(os.path.join(self.build_temp, 'python', 'tflogger.py'), extdir)

with open('README.md') as f:
    long_description = f.read()

setup(
    name='tflogger',
    version='1.0.0',
    author='Viktor Gal',
    author_email='viktor.gal@maeth.com',
    url='https://github.com/shogun-toolbox/tflogger',
    description='TFLogger is a standalone library to read and write events for TensorBoard.',
    long_description=long_description,
    long_description_content_type='text/markdown',
    ext_modules=[CMakeExtension('tflogger')],
    cmdclass=dict(build_ext=CMakeBuild),
    zip_safe=False,
    install_requires=['protobuf>=3.6'],
    classifiers=[
        'License :: OSI Approved :: Apache Software License',
        'Programming Language :: C',
        'Programming Language :: C++',
        'Programming Language :: Python',
        'Development Status :: 5 - Production/Stable',
        'Intended Audience :: Developers'
        ],
    license='Apache 2.0',
)