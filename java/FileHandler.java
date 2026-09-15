import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;
import java.util.ArrayList;
import java.util.List;

public class FileHandler {

    public static void saveQuestionsToFile(List<Question> questions, String title, File file) throws IOException {
        try (BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(
                new FileOutputStream(file), StandardCharsets.UTF_8))) {

            writer.write(title);
            writer.newLine();
            writer.newLine();

            for (Question question : questions) {
                writer.write(question.getText());
                writer.newLine();

                for (String answer : question.getAnswers()) {
                    writer.write(answer);
                    writer.newLine();
                }

                writer.newLine(); // Empty line between questions
            }
        }
    }

    public static MCQData loadQuestionsFromFile(File file) throws IOException {
        List<String> lines = Files.readAllLines(file.toPath(), StandardCharsets.UTF_8);

        String title = "";
        List<Question> questions = new ArrayList<>();

        int lineIndex = 0;

        // Read title (first non-empty line)
        while (lineIndex < lines.size() && lines.get(lineIndex).trim().isEmpty()) {
            lineIndex++;
        }
        if (lineIndex < lines.size()) {
            title = lines.get(lineIndex).trim();
            lineIndex++;
        }

        // Skip empty lines after title
        while (lineIndex < lines.size() && lines.get(lineIndex).trim().isEmpty()) {
            lineIndex++;
        }

        // Read questions
        while (lineIndex < lines.size() && questions.size() < 5) {
            String questionText = lines.get(lineIndex).trim();
            lineIndex++;

            List<String> answers = new ArrayList<>();

            // Read up to 5 answers
            while (lineIndex < lines.size() && answers.size() < 5) {
                String line = lines.get(lineIndex);
                if (line.trim().isEmpty()) {
                    lineIndex++;
                    break;
                }
                answers.add(line.trim());
                lineIndex++;
            }

            // Pad answers if fewer than 5
            while (answers.size() < 5) {
                answers.add("");
            }

            questions.add(new Question(questionText, answers));
        }

        // Pad questions if fewer than 5
        while (questions.size() < 5) {
            questions.add(new Question());
        }

        return new MCQData(title, questions);
    }

    public static void writeShuffledVersions(List<Question> questions, String title,
                                            File outputDir, boolean htmlFormat) throws IOException {
        if (!outputDir.exists()) {
            outputDir.mkdirs();
        }

        for (int version = 0; version < 5; version++) {
            String extension = htmlFormat ? ".html" : ".txt";
            File outputFile = new File(outputDir, "version" + (version + 1) + extension);

            if (htmlFormat) {
                writeHTMLVersion(questions, title, version, outputFile);
            } else {
                writeASCIIVersion(questions, title, version, outputFile);
            }
        }
    }

    private static void writeASCIIVersion(List<Question> questions, String title,
                                         int version, File file) throws IOException {
        try (BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(
                new FileOutputStream(file), StandardCharsets.UTF_8))) {

            writer.write(title);
            writer.newLine();

            for (int questionIndex = 0; questionIndex < 5; questionIndex++) {
                int shuffledQuestionIndex = LatinSquare.getShuffledIndex(questionIndex, version);
                Question question = questions.get(shuffledQuestionIndex);

                writer.newLine();
                writer.write((questionIndex + 1) + ". " + question.getText());
                writer.newLine();

                for (int answerIndex = 0; answerIndex < 5; answerIndex++) {
                    int shuffledAnswerIndex = LatinSquare.getShuffledIndex(answerIndex, version);
                    writer.write("- " + question.getAnswer(shuffledAnswerIndex));
                    writer.newLine();
                }
            }
        }
    }

    private static void writeHTMLVersion(List<Question> questions, String title,
                                        int version, File file) throws IOException {
        try (BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(
                new FileOutputStream(file), StandardCharsets.UTF_8))) {

            writer.write("<!DOCTYPE html>");
            writer.newLine();
            writer.write("<html>");
            writer.newLine();
            writer.write("<head>");
            writer.newLine();
            writer.write("<title>" + escapeHTML(title) + "</title>");
            writer.newLine();
            writer.write("</head>");
            writer.newLine();
            writer.write("<body>");
            writer.newLine();
            writer.write("<h1>" + escapeHTML(title) + "</h1>");
            writer.newLine();

            for (int questionIndex = 0; questionIndex < 5; questionIndex++) {
                int shuffledQuestionIndex = LatinSquare.getShuffledIndex(questionIndex, version);
                Question question = questions.get(shuffledQuestionIndex);

                writer.write("<h3><br>" + escapeHTML(question.getText()) + "</h3>");
                writer.newLine();
                writer.write("<table>");
                writer.newLine();

                for (int answerIndex = 0; answerIndex < 5; answerIndex++) {
                    int shuffledAnswerIndex = LatinSquare.getShuffledIndex(answerIndex, version);
                    writer.write("<tr>");
                    writer.write("<td>&#x25A1;</td>");
                    writer.write("<td>" + escapeHTML(question.getAnswer(shuffledAnswerIndex)) + "</td>");
                    writer.write("</tr>");
                    writer.newLine();
                }

                writer.write("</table>");
                writer.newLine();
            }

            writer.write("</body>");
            writer.newLine();
            writer.write("</html>");
        }
    }

    private static String escapeHTML(String text) {
        if (text == null) return "";
        return text.replace("&", "&amp;")
                   .replace("<", "&lt;")
                   .replace(">", "&gt;")
                   .replace("\"", "&quot;")
                   .replace("'", "&apos;");
    }

    public static class MCQData {
        public final String title;
        public final List<Question> questions;

        public MCQData(String title, List<Question> questions) {
            this.title = title;
            this.questions = questions;
        }
    }
}