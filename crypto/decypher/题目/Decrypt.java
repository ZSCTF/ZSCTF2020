import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Base64;
import java.util.Objects;
import java.util.Scanner;
import java.util.StringJoiner;


public class Decrypt {

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

        private static long encryptPassword(String password) {
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
        Path encryptedPath = Paths.get("flag.txt");
        if (!encryptedPath.toFile().exists()) {
            System.out.println("File not exists!");
            return;
        }

        String encryptedString = new String(Files.readAllBytes(encryptedPath));

        System.out.print("Please input password: ");
        Scanner scanner = new Scanner(System.in);
        String password = scanner.nextLine();

        try {
            Decypher.DecFile decryptedDecFile = Decypher.decrypt(encryptedString, password);
            byte[] decryptedData = Base64.getDecoder().decode(decryptedDecFile.data);
            Files.write(Paths.get("flag.txt" + decryptedDecFile.fileExtension), decryptedData);
        } catch (Exception err) {
            System.out.println("Decrypt error: " + err.getMessage());
        }
    }

}