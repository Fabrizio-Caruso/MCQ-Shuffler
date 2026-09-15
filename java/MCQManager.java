import javax.swing.*;
import javax.swing.border.*;
import javax.swing.event.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.ArrayList;
import java.util.List;

public class MCQManager extends JFrame {
    private JTextField titleField;
    private JTabbedPane questionTabs;
    private List<QuestionPanel> questionPanels;
    private JTextArea previewArea;

    public MCQManager() {
        setTitle("MCQ Manager - Multiple Choice Question Generator");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BorderLayout());

        questionPanels = new ArrayList<>();

        createUI();

        setSize(1000, 700);
        setLocationRelativeTo(null);
    }

    private void createUI() {
        // Top panel with title
        JPanel topPanel = new JPanel(new BorderLayout());
        topPanel.setBorder(new EmptyBorder(10, 10, 10, 10));

        JLabel titleLabel = new JLabel("Test Title:");
        titleField = new JTextField();
        titleField.setFont(new Font("Arial", Font.BOLD, 14));

        JPanel titlePanel = new JPanel(new BorderLayout(5, 5));
        titlePanel.add(titleLabel, BorderLayout.WEST);
        titlePanel.add(titleField, BorderLayout.CENTER);
        topPanel.add(titlePanel, BorderLayout.NORTH);

        add(topPanel, BorderLayout.NORTH);

        // Center panel with question tabs
        questionTabs = new JTabbedPane();
        for (int i = 0; i < 5; i++) {
            QuestionPanel questionPanel = new QuestionPanel(i + 1);
            questionPanels.add(questionPanel);
            questionTabs.addTab("Question " + (i + 1), questionPanel);
        }

        add(questionTabs, BorderLayout.CENTER);

        // Bottom panel with buttons and preview
        JPanel bottomPanel = new JPanel(new BorderLayout());
        bottomPanel.setBorder(new EmptyBorder(10, 10, 10, 10));

        JPanel buttonPanel = new JPanel(new FlowLayout(FlowLayout.CENTER, 10, 5));

        JButton saveButton = new JButton("Save Questions");
        saveButton.addActionListener(e -> saveQuestions());

        JButton loadButton = new JButton("Load Questions");
        loadButton.addActionListener(e -> loadQuestions());

        JButton generateASCIIButton = new JButton("Generate ASCII Versions");
        generateASCIIButton.addActionListener(e -> generateVersions(false));

        JButton generateHTMLButton = new JButton("Generate HTML Versions");
        generateHTMLButton.addActionListener(e -> generateVersions(true));

        JButton previewButton = new JButton("Preview Shuffle");
        previewButton.addActionListener(e -> showPreview());

        JButton clearButton = new JButton("Clear All");
        clearButton.addActionListener(e -> clearAll());

        buttonPanel.add(saveButton);
        buttonPanel.add(loadButton);
        buttonPanel.add(generateASCIIButton);
        buttonPanel.add(generateHTMLButton);
        buttonPanel.add(previewButton);
        buttonPanel.add(clearButton);

        bottomPanel.add(buttonPanel, BorderLayout.NORTH);

        // Preview area
        previewArea = new JTextArea(10, 40);
        previewArea.setEditable(false);
        previewArea.setFont(new Font("Monospaced", Font.PLAIN, 12));
        JScrollPane previewScroll = new JScrollPane(previewArea);
        previewScroll.setBorder(BorderFactory.createTitledBorder("Preview"));
        bottomPanel.add(previewScroll, BorderLayout.CENTER);

        add(bottomPanel, BorderLayout.SOUTH);
    }

    private List<Question> collectQuestions() {
        List<Question> questions = new ArrayList<>();
        for (QuestionPanel panel : questionPanels) {
            questions.add(panel.getQuestion());
        }
        return questions;
    }

    private void saveQuestions() {
        JFileChooser fileChooser = new JFileChooser();
        fileChooser.setDialogTitle("Save Questions");

        if (fileChooser.showSaveDialog(this) == JFileChooser.APPROVE_OPTION) {
            File file = fileChooser.getSelectedFile();
            if (!file.getName().toLowerCase().endsWith(".txt")) {
                file = new File(file.getAbsolutePath() + ".txt");
            }

            try {
                FileHandler.saveQuestionsToFile(collectQuestions(), titleField.getText(), file);
                JOptionPane.showMessageDialog(this, "Questions saved successfully!",
                                            "Success", JOptionPane.INFORMATION_MESSAGE);
            } catch (IOException ex) {
                JOptionPane.showMessageDialog(this, "Error saving file: " + ex.getMessage(),
                                            "Error", JOptionPane.ERROR_MESSAGE);
            }
        }
    }

    private void loadQuestions() {
        JFileChooser fileChooser = new JFileChooser();
        fileChooser.setDialogTitle("Load Questions");

        if (fileChooser.showOpenDialog(this) == JFileChooser.APPROVE_OPTION) {
            File file = fileChooser.getSelectedFile();

            try {
                FileHandler.MCQData data = FileHandler.loadQuestionsFromFile(file);
                titleField.setText(data.title);

                for (int i = 0; i < Math.min(data.questions.size(), questionPanels.size()); i++) {
                    questionPanels.get(i).setQuestion(data.questions.get(i));
                }

                JOptionPane.showMessageDialog(this, "Questions loaded successfully!",
                                            "Success", JOptionPane.INFORMATION_MESSAGE);
            } catch (IOException ex) {
                JOptionPane.showMessageDialog(this, "Error loading file: " + ex.getMessage(),
                                            "Error", JOptionPane.ERROR_MESSAGE);
            }
        }
    }

    private void generateVersions(boolean htmlFormat) {
        JFileChooser dirChooser = new JFileChooser();
        dirChooser.setDialogTitle("Select Output Directory");
        dirChooser.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);

        if (dirChooser.showSaveDialog(this) == JFileChooser.APPROVE_OPTION) {
            File outputDir = dirChooser.getSelectedFile();

            try {
                FileHandler.writeShuffledVersions(collectQuestions(), titleField.getText(),
                                                 outputDir, htmlFormat);

                String format = htmlFormat ? "HTML" : "ASCII";
                JOptionPane.showMessageDialog(this,
                    format + " versions generated successfully in:\n" + outputDir.getAbsolutePath(),
                    "Success", JOptionPane.INFORMATION_MESSAGE);
            } catch (IOException ex) {
                JOptionPane.showMessageDialog(this, "Error generating versions: " + ex.getMessage(),
                                            "Error", JOptionPane.ERROR_MESSAGE);
            }
        }
    }

    private void showPreview() {
        StringBuilder preview = new StringBuilder();
        List<Question> questions = collectQuestions();

        preview.append("Title: ").append(titleField.getText()).append("\n");
        preview.append("=".repeat(50)).append("\n\n");

        for (int version = 0; version < 2; version++) { // Show first 2 versions
            preview.append("VERSION ").append(version + 1).append("\n");
            preview.append("-".repeat(50)).append("\n");

            for (int qIndex = 0; qIndex < 5; qIndex++) {
                int shuffledQIndex = LatinSquare.getShuffledIndex(qIndex, version);
                Question question = questions.get(shuffledQIndex);

                preview.append("\nQ").append(qIndex + 1).append(": ")
                       .append(question.getText()).append("\n");

                for (int aIndex = 0; aIndex < 5; aIndex++) {
                    int shuffledAIndex = LatinSquare.getShuffledIndex(aIndex, version);
                    preview.append("  ").append((char)('a' + aIndex)).append(") ")
                           .append(question.getAnswer(shuffledAIndex)).append("\n");
                }
            }

            preview.append("\n");
        }

        previewArea.setText(preview.toString());
    }

    private void clearAll() {
        int confirm = JOptionPane.showConfirmDialog(this,
            "Are you sure you want to clear all questions?",
            "Clear All", JOptionPane.YES_NO_OPTION);

        if (confirm == JOptionPane.YES_OPTION) {
            titleField.setText("");
            for (QuestionPanel panel : questionPanels) {
                panel.clear();
            }
            previewArea.setText("");
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            try {
                UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
            } catch (Exception e) {
                e.printStackTrace();
            }

            new MCQManager().setVisible(true);
        });
    }
}

class QuestionPanel extends JPanel {
    private JTextArea questionArea;
    private List<JTextArea> answerAreas;
    private int questionNumber;

    public QuestionPanel(int questionNumber) {
        this.questionNumber = questionNumber;
        this.answerAreas = new ArrayList<>();

        setLayout(new BorderLayout(10, 10));
        setBorder(new EmptyBorder(10, 10, 10, 10));

        // Question text area
        JPanel questionPanel = new JPanel(new BorderLayout(5, 5));
        questionPanel.setBorder(BorderFactory.createTitledBorder("Question " + questionNumber));

        questionArea = new JTextArea(3, 50);
        questionArea.setLineWrap(true);
        questionArea.setWrapStyleWord(true);
        questionArea.setFont(new Font("Arial", Font.PLAIN, 14));
        JScrollPane questionScroll = new JScrollPane(questionArea);

        questionPanel.add(questionScroll, BorderLayout.CENTER);
        add(questionPanel, BorderLayout.NORTH);

        // Answers panel
        JPanel answersPanel = new JPanel(new GridLayout(5, 1, 5, 5));
        answersPanel.setBorder(BorderFactory.createTitledBorder("Answers"));

        for (int i = 0; i < 5; i++) {
            JPanel answerRow = new JPanel(new BorderLayout(5, 5));
            JLabel answerLabel = new JLabel(String.valueOf((char)('A' + i)) + ":");
            answerLabel.setPreferredSize(new Dimension(30, 25));

            JTextArea answerArea = new JTextArea(2, 50);
            answerArea.setLineWrap(true);
            answerArea.setWrapStyleWord(true);
            answerArea.setFont(new Font("Arial", Font.PLAIN, 13));
            answerAreas.add(answerArea);

            JScrollPane answerScroll = new JScrollPane(answerArea);
            answerRow.add(answerLabel, BorderLayout.WEST);
            answerRow.add(answerScroll, BorderLayout.CENTER);

            answersPanel.add(answerRow);
        }

        add(answersPanel, BorderLayout.CENTER);
    }

    public Question getQuestion() {
        List<String> answers = new ArrayList<>();
        for (JTextArea answerArea : answerAreas) {
            answers.add(answerArea.getText().trim());
        }
        return new Question(questionArea.getText().trim(), answers);
    }

    public void setQuestion(Question question) {
        questionArea.setText(question.getText());

        List<String> answers = question.getAnswers();
        for (int i = 0; i < Math.min(answers.size(), answerAreas.size()); i++) {
            answerAreas.get(i).setText(answers.get(i));
        }
    }

    public void clear() {
        questionArea.setText("");
        for (JTextArea answerArea : answerAreas) {
            answerArea.setText("");
        }
    }
}