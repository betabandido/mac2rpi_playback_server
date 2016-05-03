import os.path

PORTAUDIO_PATH = '/home/pi/soft/portaudio/install'
BOOST_PATH = '/home/pi/soft/boost_1_60_0/install'

env = Environment(CXXFLAGS='-std=c++14 -O2',
                  CPPPATH=[os.path.join(PORTAUDIO_PATH, 'include'),
                           os.path.join(BOOST_PATH, 'include')],
                  LIBPATH=[os.path.join(PORTAUDIO_PATH, 'lib'),
                           os.path.join(BOOST_PATH, 'lib')])

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

