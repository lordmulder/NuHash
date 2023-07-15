/******************************************************************************/
/* NuHash, by LoRd_MuldeR <MuldeR2@GMX.de>                                    */
/* This work has been released under the CC0 1.0 Universal license!           */
/******************************************************************************/

use easy_hex::Hex;
use nuhash_rs::{NuHash, nuhash_version};

fn compute(input_data: &str) {
	let mut nuhash: NuHash = NuHash::default();                // create NuHash instance
	nuhash.update(input_data.as_bytes());                      // process some input
	let digest = nuhash.do_final();                            // finalize hash computation
	println!("\"{}\" -> {}", input_data, Hex::from(digest));   // print as hex-string
}

pub fn main() {
	let version = nuhash_version();
	println!("NuHash v{}.{}.{}", version[0], version[1], version[2]);
	compute("The quick brown fox jumps over the lazy dog");
	compute("The quick brown fox jumps over the lazy cog");
}
