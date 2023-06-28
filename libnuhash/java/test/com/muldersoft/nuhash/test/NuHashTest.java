/******************************************************************************/
/* NuHash, by LoRd_MuldeR <MuldeR2@GMX.de>                                    */
/* This work has been released under the CC0 1.0 Universal license!           */
/******************************************************************************/

package com.muldersoft.nuhash.test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.lang.reflect.Field;
import java.nio.charset.StandardCharsets;
import java.util.Base64;
import java.util.List;

import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

import com.muldersoft.nuhash.NuHash;

class NuHashTest {

	@BeforeAll
	static void init() {
		final List<Integer> version = NuHash.VERSION;
		System.out.printf("NuHash v%d.%d.%d\n", version.get(0), version.get(1), version.get(2));
	}

	/* ------------------------------------------------------------------------ */
	/* TEST VECTORS                                                             */
	/* ------------------------------------------------------------------------ */

	@Test
	void testCase0() throws Exception {
		final Field field = NuHash.class.getDeclaredField("NUHASH_XOR");
		field.setAccessible(true);
		final long[][] xorTable = (long[][]) field.get(null);
		for (int i = 0; i < xorTable.length; ++i) {
			for (int j = 0; j < xorTable.length; ++j) {
				if (i != j) {
					assertTrue(computeDistance(xorTable[i], xorTable[j]) >= 182L, "Distance too small !!!");
				}
			}
		}
	}

	@Test
	void testCase1() {
		runTest(
			"d7964fe1bec2b5ecf21ecc88c86ce4f1e89fb1ef3669d52e34eb049d7fd6c42d4b2bbeeeb70d12c3fcaf43dd2229abc9");
	}

	@Test
	void testCase2() {
		runTest(
			"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
			"b3f13f534034ae8d645d410e8828673761fe2d697218879f9ea528d976ba2e15a3f7485105e612b9a46cb3988635a70f");
	}

	@Test
	void testCase3() {
		runTest(
			"abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
			"63703476f4ffce4a8c758d0d3b91a857b9a2e3433ee864f931ba328fa7247b1b0ac9c2e0279243b83051aafac6a7e710");
	}

	@Test
	void testCase4() {
		runTest(
			String.valueOf('a'),
			1000000,
			"c6808aeb911c8837cee2e6867e589ab28526a8153494cf35a40c4bd6e20312da33cac4a390cd3b51a2eb4226e60b53e1");
	}

	@Test
	void testCase5() {
		runTest(
			"abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmno",
			16777216,
			"bcca6bc8d113dd3fc171d743ac7ad2d3e63163ea7107d3c78517cf5b53ee0b2f4a0fe079e6a74df350c667b7cfe3cf09");
	}

	@Test
	void testCase6() {
		runTest(
			"g4tB,=[TU%/|8lv_c5Y?F9Sqs1PnV#bNO6DW\\0em~p<ifJhM(2&Qo}`!-*:jrZ{KGX]$zkuLd+)xwC^.\"@AIHR;y'a>3E7",
			16777216,
			"56770e133b9eb4205f11824ad7280f5c31bc04dd333d1844b3589ab8b94ab95dc09b77be9a8128d7f155fa73de617597"
		);
	}

	@Test
	void testCase7() {
		runTest(
			decode("pCxVpbBe2DPzGPunIMc01x0tRo3Kiuf8aFtU2vnDdmKpPTm/F7eyjBCiH+N0akpsepLRUuqIumvWZJae5kxHQsVR8lD0e0/ph22USyPwRRqa5JEPO7RxgpnJSeW+KJOYKtlzZVjAg0iPAxRAGyQepj/3Z6s3Bqz2ZrbdHAo1n1YV7PErfFddAu2oYxZ13JtcfQ1f/QWgbpCVhdITYQijAcjThKoJ9XBv3rkEtdswvAeO/olgJk5ZyzaGEXLixq86i3/6QTjf7i8yoSdpMSlDxJx3IUS4edCtl+DhDhkuIr2A6LH4TT7CfgyzgRLMwVPPeM481Vq7nevvzf8LJa7U"),
			16777216,
			"9a8579bb88112b2f3285781a61348d7b04a5632fec12a73a7e0e367a556794a24a32ca7bb14da68eaed83e99c2f8f4c0");
	}

	@Test
	void testCase8() {
		runTest(
			decode("tcib0bpmKRnSMO0mW8SRTRT3FjVaaf6ZpUnxlCu83EzdT8obiJyp9nMv/T9AORPas3H03n8CPWR4ehcKk9/rnwcEeYyYuCiArR/ylmu9Rdn6LFbO8IcjIurFJ9NjryTHtwbkdYN7vy6k7/USXWCVp1MRdLSgnuOuu1VyWaad1Yltj0oMHc0q++7Xw/MlYo6wssI4PGpvTjsI1IY2GFzgHoF3+GjbWFT/l35hwFHJbOWCvs8yQwE6XlBHoTTMDXCsxkSaPhAO7APibqr8fTcxS+a2wRWLBYWKZ/mrjeGiSBzLUmV2DxohqKNCQZCS0IQJLTNf2HyxuVfo1gsg50bp"),
			16777216,
			"fbf4444b0329f508bfc6cf44920b311a718e5f4c4c1af978e78223ec3579365ff5d15c16bb5a33d69cb36f3d40564757");
	}

	/* ------------------------------------------------------------------------ */
	/* TEST RUNNER                                                              */
	/* ------------------------------------------------------------------------ */

	private static void runTest(final String expected) {
		runTest("", 0, expected);
	}

	private static void runTest(final String input, final String expected) {
		runTest(input, 1, expected);
	}

	private static void runTest(final String input, final int iterations, final String expected) {
		assertNotNull(input);
		runTest(input.getBytes(StandardCharsets.UTF_8), iterations, expected);
	}

	private static void runTest(final byte[] input, final int iterations, final String expected) {
		assertNotNull(input);
		assertTrue((iterations > 0) || (input.length == 0));
		assertFalse(expected.isEmpty());
		final NuHash instance = new NuHash();
		try {
			for (int i = 0; i < iterations; ++i) {
				instance.update(input);
			}
			assertDigest(expected, instance.doFinal());
		} finally {
			instance.destroy();
		}
	}

	/* ------------------------------------------------------------------------ */
	/* UTILITIES                                                                */
	/* ------------------------------------------------------------------------ */
	
	private static byte[] decode(final String base64data) {
		return Base64.getDecoder().decode(base64data);
	}
	
	private static long computeDistance(final long[] row1, final long[] row2) {
		assertEquals(row1.length, row2.length, "Length mismatch!");
		long distance = 0;
		for (int i = 0; i < row1.length; ++i) {
			distance += Long.bitCount(row1[i] ^ row2[i]);
		}
		return distance;
	}

	private static String printHexString(final byte[] digest) {
		final StringBuilder sb = new StringBuilder(2 * digest.length);
		for(final byte b : digest) {
			sb.append(String.format("%02x", b));
		}
		return sb.toString();
	}

	private static void assertDigest(final String expected, final byte[] actual) {
		final String hexString = printHexString(actual);
		final boolean success = expected.equalsIgnoreCase(hexString);
		System.out.println(hexString + " - " + (success ? "OK" : "Failed!"));
		assertTrue(success, "Hash mismatch detected !!!");
	}
}
