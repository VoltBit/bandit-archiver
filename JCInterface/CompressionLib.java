/*
	Gud: https://www3.ntu.edu.sg/home/ehchua/programming/java/JavaNativeInterface.html
*/

public class CompressionLib {
	static {
		System.loadLibrary("compression");
	}

	private native void compress(String algorithm, String source, String destination);
	private native void decompress(String algorithm, String source, String destination);

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
			// srcPath = "/home/";
			// dstPath = "/home/";
			// algType = "LZW";
		}
		if (op.equals("e") || op.equals("encode"))
			new CompressionLib().compress(algType, srcPath, dstPath);

		if (op.equals("d") || op.equals("decode"))
			new CompressionLib().decompress(algType, srcPath, dstPath);
	}
}