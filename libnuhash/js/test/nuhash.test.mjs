/******************************************************************************/
/* NuHash, by LoRd_MuldeR <MuldeR2@GMX.de>                                    */
/* This work has been released under the CC0 1.0 Universal license!           */
/******************************************************************************/

import { NuHash, toHexStr } from "../lib/nuhash.mjs";
import * as assert from "assert";
import setTitle from "console-title";

function runTestCase(self, iterations, expected, src) {
    self.timeout(Number.MAX_VALUE);
    if ((src instanceof String) || (typeof(src) === 'string')) {
        src = new TextEncoder().encode(src);
    }
    let nuhash = new NuHash();
    for (let iter = 0; iter < iterations; ++iter) {
        nuhash.update(src);
        if ((iter & 0x7FFF) == 0) {
            setTitle((iter / iterations * 100.0).toFixed(2) + '%');
        }
    }
    const digest = toHexStr(nuhash.doFinal());
    console.log(digest);
    assert.equal(expected, digest);
}

function decode(base64String) {
    return Uint8Array.from(Buffer.from(base64String, 'base64'));
}

describe('NuHash Tests', function () {

    it('Test #1', function () {
        runTestCase(
            this,
            0,
            "d7964fe1bec2b5ecf21ecc88c86ce4f1e89fb1ef3669d52e34eb049d7fd6c42d4b2bbeeeb70d12c3fcaf43dd2229abc9",
            new Uint8Array());
    });

    it('Test #2', function () {
        runTestCase(
            this,
            1,
            "b3f13f534034ae8d645d410e8828673761fe2d697218879f9ea528d976ba2e15a3f7485105e612b9a46cb3988635a70f",
            "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq");
    });

    it('Test #3', function () {
        runTestCase(
            this,
            1,
            "63703476f4ffce4a8c758d0d3b91a857b9a2e3433ee864f931ba328fa7247b1b0ac9c2e0279243b83051aafac6a7e710",
            "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu");
    });

    it('Test #4', function () {
        runTestCase(
            this,
            0xF4240,
            "c6808aeb911c8837cee2e6867e589ab28526a8153494cf35a40c4bd6e20312da33cac4a390cd3b51a2eb4226e60b53e1",
            "a");
    });

    it('Test #5', function () {
        runTestCase(
            this,
            16777216,
            "bcca6bc8d113dd3fc171d743ac7ad2d3e63163ea7107d3c78517cf5b53ee0b2f4a0fe079e6a74df350c667b7cfe3cf09",
            "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmno");
    });

    it('Test #6', function () {
        runTestCase(
            this,
            16777216,
            "56770e133b9eb4205f11824ad7280f5c31bc04dd333d1844b3589ab8b94ab95dc09b77be9a8128d7f155fa73de617597",
            "g4tB,=[TU%/|8lv_c5Y?F9Sqs1PnV#bNO6DW\\0em~p<ifJhM(2&Qo}`!-*:jrZ{KGX]$zkuLd+)xwC^.\"@AIHR;y'a>3E7");
    });

    it('Test #7', function () {
        runTestCase(
            this,
            16777216,
            "9a8579bb88112b2f3285781a61348d7b04a5632fec12a73a7e0e367a556794a24a32ca7bb14da68eaed83e99c2f8f4c0",
            decode("pCxVpbBe2DPzGPunIMc01x0tRo3Kiuf8aFtU2vnDdmKpPTm/F7eyjBCiH+N0akpsepLRUuqIumvWZJae5kxHQsVR8lD0e0/ph22USyPwRRqa5JEPO7RxgpnJSeW+KJOYKtlzZVjAg0iPAxRAGyQepj/3Z6s3Bqz2ZrbdHAo1n1YV7PErfFddAu2oYxZ13JtcfQ1f/QWgbpCVhdITYQijAcjThKoJ9XBv3rkEtdswvAeO/olgJk5ZyzaGEXLixq86i3/6QTjf7i8yoSdpMSlDxJx3IUS4edCtl+DhDhkuIr2A6LH4TT7CfgyzgRLMwVPPeM481Vq7nevvzf8LJa7U"));
    });

    it('Test #8', function () {
        runTestCase(
            this,
            16777216,
            "fbf4444b0329f508bfc6cf44920b311a718e5f4c4c1af978e78223ec3579365ff5d15c16bb5a33d69cb36f3d40564757",
            decode("tcib0bpmKRnSMO0mW8SRTRT3FjVaaf6ZpUnxlCu83EzdT8obiJyp9nMv/T9AORPas3H03n8CPWR4ehcKk9/rnwcEeYyYuCiArR/ylmu9Rdn6LFbO8IcjIurFJ9NjryTHtwbkdYN7vy6k7/USXWCVp1MRdLSgnuOuu1VyWaad1Yltj0oMHc0q++7Xw/MlYo6wssI4PGpvTjsI1IY2GFzgHoF3+GjbWFT/l35hwFHJbOWCvs8yQwE6XlBHoTTMDXCsxkSaPhAO7APibqr8fTcxS+a2wRWLBYWKZ/mrjeGiSBzLUmV2DxohqKNCQZCS0IQJLTNf2HyxuVfo1gsg50bp"));
    });
});
