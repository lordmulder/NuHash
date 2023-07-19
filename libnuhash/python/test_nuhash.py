#!/usr/bin/env python3

"""NuHash, by LoRd_MuldeR <MuldeR2@GMX.de>
This work has been released under the CC0 1.0 Universal license!"""

from nuhash import NuHash

import ctypes
import sys
import unittest

# ========================================================================
# TEST FUNCTIONS
# ========================================================================

class NuHashTests(unittest.TestCase):

    @staticmethod
    def set_term_title(text):
        if sys.platform.lower().startswith(('win32', 'msys', 'cygwin')):
            ctypes.windll.kernel32.SetConsoleTitleW(text)
        elif sys.platform.lower().startswith(('linux', 'freebsd', 'openbsd', 'netbsd', 'darwin', 'aix')):
            sys.stdout.write("\33]0;{}\a".format(text))
            sys.stdout.flush()

    def run_testcase(self, iterations, expected, input):
        if isinstance(input, str):
            input = input.encode(encoding='UTF-8', errors='strict')
        nuhash = NuHash()
        for _iter in range(iterations):
            nuhash.update(input)
            if _iter & 0xFFFF == 0:
                self.set_term_title("{:.2f}%".format(_iter / iterations * 100.0))
        digest = nuhash.do_final().hex()
        self.set_term_title("100%")
        print("{} - ".format(digest), end = '', sep = '', flush = True)
        self.assertEqual(expected.lower(), digest.lower())

    # ------------------------------------------------------------------------
    # Test cases
    # ------------------------------------------------------------------------

    def test_function1(self):
        self.run_testcase(
            0x0000001,
            "d7964fe1bec2b5ecf21ecc88c86ce4f1e89fb1ef3669d52e34eb049d7fd6c42d4b2bbeeeb70d12c3fcaf43dd2229abc9",
            "")

    def test_function2(self):
        self.run_testcase(
            0x0000001,
            "b3f13f534034ae8d645d410e8828673761fe2d697218879f9ea528d976ba2e15a3f7485105e612b9a46cb3988635a70f",
            "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq")

    def test_function3(self):
        self.run_testcase(
            0x0000001,
            "63703476f4ffce4a8c758d0d3b91a857b9a2e3433ee864f931ba328fa7247b1b0ac9c2e0279243b83051aafac6a7e710",
            "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu")

    def test_function4(self):
        self.run_testcase(
            0x00F4240,
            "c6808aeb911c8837cee2e6867e589ab28526a8153494cf35a40c4bd6e20312da33cac4a390cd3b51a2eb4226e60b53e1",
            "a")

    def test_function5(self):
        self.run_testcase(
            0x1000000,
            "bcca6bc8d113dd3fc171d743ac7ad2d3e63163ea7107d3c78517cf5b53ee0b2f4a0fe079e6a74df350c667b7cfe3cf09",
            "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmno")

    def test_function6(self):
        self.run_testcase(
            0x1000000,
            "56770e133b9eb4205f11824ad7280f5c31bc04dd333d1844b3589ab8b94ab95dc09b77be9a8128d7f155fa73de617597",
            "g4tB,=[TU%/|8lv_c5Y?F9Sqs1PnV#bNO6DW\\0em~p<ifJhM(2&Qo}`!-*:jrZ{KGX]$zkuLd+)xwC^.\"@AIHR;y'a>3E7")

    def test_function7(self):
        self.run_testcase(
            0x1000000,
            "9a8579bb88112b2f3285781a61348d7b04a5632fec12a73a7e0e367a556794a24a32ca7bb14da68eaed83e99c2f8f4c0",
            b'\xa4,U\xa5\xb0^\xd83\xf3\x18\xfb\xa7 \xc74\xd7\x1d-F\x8d\xca\x8a\xe7\xfch[T\xda\xf9\xc3vb\xa9=9\xbf\x17\xb7\xb2\x8c\x10\xa2\x1f\xe3tjJlz\x92\xd1R\xea\x88\xbak\xd6d\x96\x9e\xe6LGB\xc5Q\xf2P\xf4{O\xe9\x87m\x94K#\xf0E\x1a\x9a\xe4\x91\x0f;\xb4q\x82\x99\xc9I\xe5\xbe(\x93\x98*\xd9seX\xc0\x83H\x8f\x03\x14@\x1b$\x1e\xa6?\xf7g\xab7\x06\xac\xf6f\xb6\xdd\x1c\n5\x9fV\x15\xec\xf1+|W]\x02\xed\xa8c\x16u\xdc\x9b\\}\r_\xfd\x05\xa0n\x90\x95\x85\xd2\x13a\x08\xa3\x01\xc8\xd3\x84\xaa\t\xf5po\xde\xb9\x04\xb5\xdb0\xbc\x07\x8e\xfe\x89`&NY\xcb6\x86\x11r\xe2\xc6\xaf:\x8b\x7f\xfaA8\xdf\xee/2\xa1\'i1)C\xc4\x9cw!D\xb8y\xd0\xad\x97\xe0\xe1\x0e\x19."\xbd\x80\xe8\xb1\xf8M>\xc2~\x0c\xb3\x81\x12\xcc\xc1S\xcfx\xce<\xd5Z\xbb\x9d\xeb\xef\xcd\xff\x0b%\xae\xd4')

    def test_function8(self):
        self.run_testcase(
            0x1000000,
            "fbf4444b0329f508bfc6cf44920b311a718e5f4c4c1af978e78223ec3579365ff5d15c16bb5a33d69cb36f3d40564757",
            b'\xb5\xc8\x9b\xd1\xbaf)\x19\xd20\xed&[\xc4\x91M\x14\xf7\x165Zi\xfe\x99\xa5I\xf1\x94+\xbc\xdcL\xddO\xca\x1b\x88\x9c\xa9\xf6s/\xfd?@9\x13\xda\xb3q\xf4\xde\x7f\x02=dxz\x17\n\x93\xdf\xeb\x9f\x07\x04y\x8c\x98\xb8(\x80\xad\x1f\xf2\x96k\xbdE\xd9\xfa,V\xce\xf0\x87#"\xea\xc5\'\xd3c\xaf$\xc7\xb7\x06\xe4u\x83{\xbf.\xa4\xef\xf5\x12]`\x95\xa7S\x11t\xb4\xa0\x9e\xe3\xae\xbbUrY\xa6\x9d\xd5\x89m\x8fJ\x0c\x1d\xcd*\xfb\xee\xd7\xc3\xf3%b\x8e\xb0\xb2\xc28<joN;\x08\xd4\x866\x18\\\xe0\x1e\x81w\xf8h\xdbXT\xff\x97~a\xc0Q\xc9l\xe5\x82\xbe\xcf2C\x01:^PG\xa14\xcc\rp\xac\xc6D\x9a>\x10\x0e\xec\x03\xe2n\xaa\xfc}71K\xe6\xb6\xc1\x15\x8b\x05\x85\x8ag\xf9\xab\x8d\xe1\xa2H\x1c\xcbRev\x0f\x1a!\xa8\xa3BA\x90\x92\xd0\x84\t-3_\xd8|\xb1\xb9W\xe8\xd6\x0b \xe7F\xe9')

# ========================================================================
# MAIN()
# ========================================================================

if __name__ == '__main__':
    if sys.hexversion < 0x3000000:
        print("Python version 3.0 or newer is required !!!")
        sys.exit(1)
    test_suite = unittest.TestSuite()
    for name in filter(lambda s: s.startswith('test_function'), dir(NuHashTests)):
        test_suite.addTest(NuHashTests(name))
    runner = unittest.TextTestRunner(verbosity = 2)
    runner.run(test_suite)
