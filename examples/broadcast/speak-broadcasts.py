#!/usr/bin/env python
from socket import *
from tempfile import NamedTemporaryFile
from subprocess import Popen as popen
import pyaudio
import wave

'''
The audio device index used for audio output.
Reference pyaudio.PyAudio() get_device_count() and get_device_info_by_index()
methods if no audio is heard.
'''
AUDIO_DEVICE_INDEX = 0

'''
Number of bytes in one read/write cycle to the audio stream.  Defaults to a
conservative 32 bytes.
'''
AUDIO_CHUNK = 32

'''
The text2wave program is provided with festival and is suitable here.  However,
any means of writing a wave file is fine.  Simply redefine WRITE_WAV_CMD.
'''
WAV_FILE = NamedTemporaryFile(mode = 'w+b')
TEXT2WAV = '/usr/share/doc/festival-1.96_beta/examples/text2wave'
TEXT2WAV_ARGS = '-o {0}'.format(WAV_FILE.name)
TEXT2WAV_CMD = TEXT2WAV + ' ' + TEXT2WAV_ARGS
WRITE_WAV_CMD = 'echo "{0}" | {1} 2>/dev/null'

IP = "224.0.0.3" 
PORT = 47002

sock = socket(AF_INET, SOCK_DGRAM)
sock.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
sock.setsockopt(SOL_IP, IP_MULTICAST_LOOP, 1)
sock.bind(('', PORT))

intf = gethostbyname(gethostname())
sock.setsockopt(SOL_IP, IP_MULTICAST_IF, inet_aton(intf))
sock.setsockopt(SOL_IP, IP_ADD_MEMBERSHIP, inet_aton(IP) + inet_aton(intf))

audio = pyaudio.PyAudio()

wav_file = None
audio_stream = None

print 'Running, interrupt to exit.'
try:
    while True:
        data, addr = sock.recvfrom(256)
        process = popen(WRITE_WAV_CMD.format(data, TEXT2WAV_CMD), shell=True)
        process.wait()
        wav_file = wave.open(WAV_FILE.name, 'rb')
        fmt = audio.get_format_from_width(wav_file.getsampwidth())

        audio_stream = audio.open(output_device_index = AUDIO_DEVICE_INDEX,
                format = audio.get_format_from_width(wav_file.getsampwidth()),
                channels = wav_file.getnchannels(),
                rate = wav_file.getframerate(),
                output = True)
        
        audio_data = wav_file.readframes(AUDIO_CHUNK)
        print 'speaking "{0}"'.format(data)
        while audio_data != '':
            audio_stream.write(audio_data)
            audio_data = wav_file.readframes(AUDIO_CHUNK)

        audio_stream.close()
        wav_file.close()
except KeyboardInterrupt:
    if audio_stream:
        audio_stream.close()
    if wav_file:
        wav_file.close()

audio.terminate()

