import java.util.ArrayList;
import java.util.List;

public class Question {
    private String text;
    private List<String> answers;

    public Question() {
        this.text = "";
        this.answers = new ArrayList<>();
        for (int i = 0; i < 5; i++) {
            answers.add("");
        }
    }

    public Question(String text, List<String> answers) {
        this.text = text;
        this.answers = new ArrayList<>(answers);
    }

    public String getText() {
        return text;
    }

    public void setText(String text) {
        this.text = text;
    }

    public List<String> getAnswers() {
        return answers;
    }

    public void setAnswers(List<String> answers) {
        this.answers = new ArrayList<>(answers);
    }

    public String getAnswer(int index) {
        if (index >= 0 && index < answers.size()) {
            return answers.get(index);
        }
        return "";
    }

    public void setAnswer(int index, String answer) {
        if (index >= 0 && index < answers.size()) {
            answers.set(index, answer);
        }
    }

    @Override
    public String toString() {
        return text.isEmpty() ? "Untitled Question" : text;
    }
}