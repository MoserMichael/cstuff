import os
from setuptools import setup
from setuptools import Extension


def read(fname):
    with open(os.path.join(os.path.dirname(__file__), fname)) as f:
        return f.read()

setup(
    name = "ntrace",
    version = "0.0.1",
    author = "Michael Moser",
    author_email = "moser.michael@gmail.com",
    description = ("execution tracer that gives more output than trace.py"),
    license = "BSD",                                                            
    keywords = "program tracing, debugging",
    url = "http://mosermichael.github.io/cstuff/all/projects/2015/02/24/python-backtrace.html",
    packages=['ntrace', 'tests'],
    long_description=read('README'),
    ext_modules = [Extension(name = "ntracenative",
                             sources = ["ntrace/ntracenativemodule.c"])],
    classifiers=[
	"Topic :: Software Development :: Debuggers",
        "Programming Language :: Python :: 2.7",
	"Intended Audience :: Developers",
 	"Operating System :: OS Independent",
        "License :: OSI Approved :: BSD License",
    ],
)
                                           
