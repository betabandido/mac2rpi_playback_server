env = Environment(CXXFLAGS='-std=c++14',
                  CPPPATH='/home/pi/soft/portaudio/install/include',
                  LIBPATH='/home/pi/soft/portaudio/install/lib')

if not env.GetOption('clean'):
  conf = Configure(env)
  if not conf.CheckLibWithHeader(
      'portaudiocpp', 'portaudiocpp/PortAudioCpp.hxx', 'cxx'):
    print 'Could not find PortAudio'
    Exit(1)
  if not conf.CheckLibWithHeader(
      'pthread', 'pthread.h', 'cxx'):
    print 'Could not find pthreads'
    Exit(1)
  if not conf.CheckLibWithHeader(
      'boost_system', 'boost/asio.hpp', 'cxx'):
    print 'Could not find boost::asio'
    Exit(1)
  env = conf.Finish()

SConscript('src/SConscript', 'env', variant_dir='build', duplicate=0)

