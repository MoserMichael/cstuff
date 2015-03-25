import os
from setuptools import setup


def read(fname):
    with open(os.path.join(os.path.dirname(__file__), fname)) as f:
        return f.read()

setup(
    name = "roget", 
    version = "0.0.1",
    author = "Michael Moser",
    author_email = "moser.michael@gmail.com",
    description = ("API to the Roget thesaurus"),
    license = "BSD",                                                                
    keywords = "natural language processing; thesaurus",
    url = "http://mosermichael.github.io/cstuff/all/projects/2015/02/26/roget.html",
    packages=['roget', 'tests'],
    long_description=read('README'),
    zip_safe=False,
    include_package_data=True,
    classifiers=[
        "Natural Language :: English",
	"Topic :: Text Processing :: Linguistic",
        "Intended Audience :: Science/Research",
	"Intended Audience :: Developers",
 	"Operating System :: OS Independent",
	"License :: OSI Approved :: BSD License",
    ],
)
