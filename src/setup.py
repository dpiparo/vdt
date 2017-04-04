#python setup.py build_ext --inplace
def configuration(parent_package='', top_path=None):
    import numpy
    from numpy.distutils.misc_util import Configuration

    config = Configuration('npufunc_directory',
                           parent_package,
                           top_path)
    config.add_extension('npufunc', ['expfWrapper.c'],    extra_link_args=['-Wl,-v'],library_dirs='./' ,libraries=["vdtdiag_numpyWrapper"],runtime_library_dirs=['./'])
    return config

if __name__ == "__main__":
    from numpy.distutils.core import setup
    setup(configuration=configuration)
