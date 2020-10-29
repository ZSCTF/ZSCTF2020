import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Base64;
import java.util.Objects;
import java.util.StringJoiner;

public class Encrypt {

    public static class Decypher {

        private static final String FILE_HEADER = "#DEC_ENC";
        private static final String DELIMETER = "::";

        public static class DecFile {

            public String fileExtension;
            public String data;

            public DecFile(String fileExtension, String data) {
                this.fileExtension = fileExtension;
                this.data = data;
            }

            @Override
            public boolean equals(Object o) {
                if (this == o) {
                    return true;
                }
                if (o == null || getClass() != o.getClass()) {
                    return false;
                }
                DecFile decFile = (DecFile)o;
                return fileExtension.equals(decFile.fileExtension) &&
                        data.equals(decFile.data);
            }

            @Override
            public int hashCode() {
                return Objects.hash(fileExtension, data);
            }

            @Override
            public String toString() {
                return new StringJoiner(", ", DecFile.class.getSimpleName() + "[", "]")
                        .add("fileExtension='" + fileExtension + "'")
                        .add("data='" + data + "'")
                        .toString();
            }

        }

        public static long encryptPassword(String password) {
            long hash1 = 0x1505;
            long hash2 = hash1;

            for (int i = 0; i < password.length(); i += 2) {
                hash1 = ((hash1 << 5) + hash1) ^ password.charAt(i);
                if (i == password.length() - 1) {
                    break;
                }
                hash2 = ((hash2 << 5) + hash2) ^ password.charAt(i + 1);
            }

            long hash = hash1 + hash2 * 0x15051505;
            return (hash % 0x33333333333333L + 0x33333333333333L) % 0x33333333333333L;
        }

        private static String encryptString(String data, long encryptedPassword) {
            StringJoiner joiner = new StringJoiner(" ");

            for (char i : data.toCharArray()) {
                long code = (long)i * 1145141919810L + 1926081719260817L + encryptedPassword;
                joiner.add(String.valueOf(code));
            }
            return joiner.toString();
        }

        private static String decryptString(String data, long encryptedPassword) {
            StringBuilder builder = new StringBuilder();
            String[] array = data.split(" +");
            for (String numString : array) {
                long num = Long.parseLong(numString);
                num = (num - encryptedPassword - 1926081719260817L) / 1145141919810L;
                builder.append((char)num);
            }
            return builder.toString();
        }

        public static String encrypt(String data, String fileExtension) {
            return encrypt(data, fileExtension, "");
        }

        public static String encrypt(String data, String fileExtension, String password) {
            long encryptedPassword = encryptPassword(password);
            long encryptedEmptyPassword = encryptPassword("");

            String encrypted = new StringJoiner(DELIMETER)
                    .add(FILE_HEADER)
                    .add(encryptString("ENCODED", encryptedPassword))
                    .add(encryptString(fileExtension, encryptedEmptyPassword))
                    .add(encryptString(data, encryptedPassword))
                    .toString();
            return encrypted;
        }

        public static DecFile decrypt(String encrypted, String password) {
            long encryptedPassword = encryptPassword(password);
            long encryptedEmptyPassword = encryptPassword("");

            String[] chunks = encrypted.split(DELIMETER, -1);
            if (chunks.length != 4 || !chunks[0].equals(FILE_HEADER)) {
                throw new IllegalArgumentException("not a valid file");
            }
            if (!"ENCODED".equals(decryptString(chunks[1], encryptedPassword))) {
                throw new IllegalArgumentException("password not correct");
            }

            String fileExtension = decryptString(chunks[2], encryptedEmptyPassword);
            String data = decryptString(chunks[3], encryptedPassword);
            return new DecFile(fileExtension, data);
        }

    }

    public static void main(String[] args) throws IOException {
        Path flagImagePath = Paths.get("题解", "flag.jpg");
        Path encryptedPath = Paths.get("题目", "flag.txt");

        String fileName = flagImagePath.toFile().getName();
        String fileExtension = fileName.substring(fileName.lastIndexOf('.'));
        String password = ";EYj6i!H8G>yXc\"qV?`v(cdz1|\"8B:n=Yy1uBWLB";
        Decypher.DecFile originalDecFile = new Decypher.DecFile(fileExtension, password);

        byte[] data = Files.readAllBytes(flagImagePath);
        String dataString = Base64.getEncoder().encodeToString(data);
        String encrypted = Decypher.encrypt(dataString, fileExtension, password);
        Files.write(encryptedPath, encrypted.getBytes());

        String encryptedString = new String(Files.readAllBytes(encryptedPath));
        Decypher.DecFile decryptedDecFile = Decypher.decrypt(encryptedString, password);
        byte[] decryptedData = Base64.getDecoder().decode(decryptedDecFile.data);
        Files.write(Paths.get("flag.txt" + decryptedDecFile.fileExtension), decryptedData);

        assert originalDecFile.equals(decryptedDecFile);
    }

}