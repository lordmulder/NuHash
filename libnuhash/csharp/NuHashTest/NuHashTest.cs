/******************************************************************************/
/* NuHash, by LoRd_MuldeR <MuldeR2@GMX.de>                                    */
/* This work has been released under the CC0 1.0 Universal license!           */
/******************************************************************************/

using LibNuHash;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Numerics;
using System.Reflection;
using System.Text;

namespace LibNuHashTest
{
    [TestClass]
    public class NuHashTest
    {
        /* ------------------------------------------------------------------------ */
        /* Test functions                                                           */
        /* ------------------------------------------------------------------------ */

        private static void RunTest(uint iterations, string expected, byte[] input)
        {
            using (NuHash nuhash = new NuHash())
            {
                for (uint iter = 0; iter < iterations; ++iter)
                {
                    nuhash.Update(input);
                }
                string digest = BitConverter.ToString(nuhash.DoFinal());
                Console.Out.WriteLine(string.Format("{0} - {1}", digest, expected.Equals(digest, StringComparison.OrdinalIgnoreCase) ? "OK" : "Failed!"));
                Assert.AreEqual(expected, digest, true);
            }
        }

        private static void RunTest(uint iterations, string expected, string input)
        {
            RunTest(iterations, expected, Encoding.UTF8.GetBytes(input));
        }

        /* ------------------------------------------------------------------------ */
        /* Test cases                                                               */
        /* ------------------------------------------------------------------------ */

        [TestMethod]
        public void TestMethod0()
        {
            ulong[][] xorTable = (ulong[][]) typeof(NuHash).GetField("NUHASH_XOR", BindingFlags.NonPublic | BindingFlags.Static).GetValue(null);
            for (int i = 0; i < 257; ++i)
            {
                for (int j = 0; j < 257; ++j)
                {
                    int distance = ComputeDistance(xorTable[i], xorTable[j]);
                    Assert.IsTrue((i == j) || (distance >= 182));
                }
            }
        }

        [TestMethod]
        public void TestMethod1()
        {
            RunTest(
                0x0000001,
                "D7-96-4F-E1-BE-C2-B5-EC-F2-1E-CC-88-C8-6C-E4-F1-E8-9F-B1-EF-36-69-D5-2E-34-EB-04-9D-7F-D6-C4-2D-4B-2B-BE-EE-B7-0D-12-C3-FC-AF-43-DD-22-29-AB-C9",
                ""
            );
        }

        [TestMethod]
        public void TestMethod2()
        {
            RunTest(
                0x0000001,
                "B3-F1-3F-53-40-34-AE-8D-64-5D-41-0E-88-28-67-37-61-FE-2D-69-72-18-87-9F-9E-A5-28-D9-76-BA-2E-15-A3-F7-48-51-05-E6-12-B9-A4-6C-B3-98-86-35-A7-0F",
                "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"
            );
        }

        [TestMethod]
        public void TestMethod3()
        {
            RunTest(
                0x0000001,
                "63-70-34-76-F4-FF-CE-4A-8C-75-8D-0D-3B-91-A8-57-B9-A2-E3-43-3E-E8-64-F9-31-BA-32-8F-A7-24-7B-1B-0A-C9-C2-E0-27-92-43-B8-30-51-AA-FA-C6-A7-E7-10",
                "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu"
            );
        }

        [TestMethod]
        public void TestMethod4()
        {
            RunTest(
                0x00F4240,
                "C6-80-8A-EB-91-1C-88-37-CE-E2-E6-86-7E-58-9A-B2-85-26-A8-15-34-94-CF-35-A4-0C-4B-D6-E2-03-12-DA-33-CA-C4-A3-90-CD-3B-51-A2-EB-42-26-E6-0B-53-E1",
                "a"
            );
        }

        [TestMethod]
        public void TestMethod5()
        {
            RunTest(
                0x1000000,
                "BC-CA-6B-C8-D1-13-DD-3F-C1-71-D7-43-AC-7A-D2-D3-E6-31-63-EA-71-07-D3-C7-85-17-CF-5B-53-EE-0B-2F-4A-0F-E0-79-E6-A7-4D-F3-50-C6-67-B7-CF-E3-CF-09",
                "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmno"
            );
        }

        [TestMethod]
        public void TestMethod6()
        {
            RunTest(
                0x1000000,
                "56-77-0E-13-3B-9E-B4-20-5F-11-82-4A-D7-28-0F-5C-31-BC-04-DD-33-3D-18-44-B3-58-9A-B8-B9-4A-B9-5D-C0-9B-77-BE-9A-81-28-D7-F1-55-FA-73-DE-61-75-97",
                "g4tB,=[TU%/|8lv_c5Y?F9Sqs1PnV#bNO6DW\\0em~p<ifJhM(2&Qo}`!-*:jrZ{KGX]$zkuLd+)xwC^.\"@AIHR;y'a>3E7"
            );
        }

        [TestMethod]
        public void TestMethod7()
        {
            RunTest(
                0x1000000,
                "9A-85-79-BB-88-11-2B-2F-32-85-78-1A-61-34-8D-7B-04-A5-63-2F-EC-12-A7-3A-7E-0E-36-7A-55-67-94-A2-4A-32-CA-7B-B1-4D-A6-8E-AE-D8-3E-99-C2-F8-F4-C0",
                Decode("pCxVpbBe2DPzGPunIMc01x0tRo3Kiuf8aFtU2vnDdmKpPTm/F7eyjBCiH+N0akpsepLRUuqIumvWZJae5kxHQsVR8lD0e0/ph22USyPwRRqa5JEPO7RxgpnJSeW+KJOYKtlzZVjAg0iPAxRAGyQepj/3Z6s3Bqz2ZrbdHAo1n1YV7PErfFddAu2oYxZ13JtcfQ1f/QWgbpCVhdITYQijAcjThKoJ9XBv3rkEtdswvAeO/olgJk5ZyzaGEXLixq86i3/6QTjf7i8yoSdpMSlDxJx3IUS4edCtl+DhDhkuIr2A6LH4TT7CfgyzgRLMwVPPeM481Vq7nevvzf8LJa7U")
            );
        }

        [TestMethod]
        public void TestMethod8()
        {
            RunTest(
                0x1000000,
                "FB-F4-44-4B-03-29-F5-08-BF-C6-CF-44-92-0B-31-1A-71-8E-5F-4C-4C-1A-F9-78-E7-82-23-EC-35-79-36-5F-F5-D1-5C-16-BB-5A-33-D6-9C-B3-6F-3D-40-56-47-57",
                Decode("tcib0bpmKRnSMO0mW8SRTRT3FjVaaf6ZpUnxlCu83EzdT8obiJyp9nMv/T9AORPas3H03n8CPWR4ehcKk9/rnwcEeYyYuCiArR/ylmu9Rdn6LFbO8IcjIurFJ9NjryTHtwbkdYN7vy6k7/USXWCVp1MRdLSgnuOuu1VyWaad1Yltj0oMHc0q++7Xw/MlYo6wssI4PGpvTjsI1IY2GFzgHoF3+GjbWFT/l35hwFHJbOWCvs8yQwE6XlBHoTTMDXCsxkSaPhAO7APibqr8fTcxS+a2wRWLBYWKZ/mrjeGiSBzLUmV2DxohqKNCQZCS0IQJLTNf2HyxuVfo1gsg50bp")
            );
        }

        /* ------------------------------------------------------------------------ */
        /* Utilities                                                                */
        /* ------------------------------------------------------------------------ */

        private static int ComputeDistance(ulong[] array1, ulong[] array2)
        {
            Assert.AreEqual(array1.Length, array2.Length);
            int distance = 0;
            for (int i = 0; i < array1.Length; ++i)
            {
                distance += BitOperations.PopCount(array1[i] ^ array2[i]);
            }
            return distance;
        }

        private static byte[] Decode(string base64string) => Convert.FromBase64String(base64string);
    }
}
