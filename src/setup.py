#python setup.py build_ext --inplace
def configuration(parent_package='', top_path=None):
    import numpy
    from numpy.distutils.misc_util import Configuration

    config = Configuration('vdtnpfun_directory',
                           parent_package,
                           top_path)
    config.add_extension('vdtnpfun', ['vdtNumpyWrapper.c'],    extra_link_args=[],library_dirs='./' ,libraries=["vdtFatLibWrapper"],runtime_library_dirs=['./'])
    return config

if __name__ == "__main__":
    from numpy.distutils.core import setup
    setup(configuration=configuration)
