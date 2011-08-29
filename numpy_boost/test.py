import numpy

def test():
    from numpy_boost import _numpy_boost_test

    x = numpy.zeros((3, 4, 6), order="C")
    print x.strides
    for i in range(x.shape[0]):
        for j in range(x.shape[1]):
            for k in range(x.shape[2]):
                x[i][j][k] = (i+1)*(j+1)*(k+1)

    # Do a funny striding on the array
    strided = x[::2,1:2,::3]

    print "Python"
    for i in range(strided.shape[0]):
        print '[',
        for j in range(strided.shape[1]):
            print '[',
            for k in range(strided.shape[2]):
                print strided[i][j][k],
            print ']',
        print ']',

    print

    result = _numpy_boost_test.test(strided)

    print "result"
    print result

    try:
        result = _numpy_boost_test.test(None)
    except ValueError:
        pass
    else:
        assert False, "Exception not thrown"

def test_boost_python():
    try:
        from numpy_boost import _numpy_boost_python_test
    except ImportError:
        return

    x = numpy.zeros((3, 4, 6), order="C")
    print x.strides
    for i in range(x.shape[0]):
        for j in range(x.shape[1]):
            for k in range(x.shape[2]):
                x[i][j][k] = (i+1)*(j+1)*(k+1)

    # Do a funny striding on the array
    strided = x[::2,1:2,::3]

    result = _numpy_boost_python_test.test(strided)

    print "result"
    print result

if __name__ == "__main__":
    test()
    test_boost_python()
