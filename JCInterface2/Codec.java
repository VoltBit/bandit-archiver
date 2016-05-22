import java.util.*;
import java.io.*;

public class Codec {
	void run_codec(String srcPath, String dstPath, String alg, String op) {
		Runtime rt = Runtime.getRuntime();
		String command;

		if (alg.equals("lzw")) {
			command = "../LZW/lzw ";
		} else if (alg.equals("huff")) {
			command = "../Huffman/huffman ";
		} else {
			System.out.print("Unknown algorithm");
			return;
		}
		command = command + op + " " + srcPath + " " + dstPath;
		Process ps = rt.exec(command);
	}
}

class Test {
	public static void main(String[] args) {
		String op, srcPath, dstPath, algType;
		if (args.length == 4) {
			op = args[0];
			algType = args[1];
			srcPath = args[2];
			dstPath = args[3];
		} else {
			System.out.print("Wrong usage.\n<op_type> <alg_type> <source> <destination>\n");
			return;
		}
		new Codec().run_codec(srcPath, dstPath, algType, op);
	}
}