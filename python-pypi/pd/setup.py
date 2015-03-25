import os
from setuptools import setup


def read(fname):
    with open(os.path.join(os.path.dirname(__file__), fname)) as f:
        return f.read()

setup(
    name = "pd",
    version = "0.0.1",
    author = "Michael Moser",
    author_email = "moser.michael@gmail.com",
    description = ("more detailed python backtraces (similar to backtrace module)"),
    license = "BSD",                                                            
    keywords = "backtrace, debugging",
    url = "http://mosermichael.github.io/cstuff/all/projects/2015/02/24/python-backtrace.html",
    packages=['pd', 'tests'],
    long_description=read('README'),
    zip_safe=False,
    classifiers=[
        "Topic :: Utilities",
	"Intended Audience :: Developers",
 	"Operating System :: OS Independent",
        "License :: OSI Approved :: BSD License",
    ],
)
