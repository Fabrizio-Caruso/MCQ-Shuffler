public class LatinSquare {
    // 5x5 non-cyclic Latin Square for shuffling
    private static final int[][] SHUFFLE = {
        {0, 1, 2, 3, 4},
        {1, 2, 4, 0, 3},
        {2, 4, 3, 1, 0},
        {3, 0, 1, 4, 2},
        {4, 3, 0, 2, 1}
    };

    public static int getShuffledIndex(int originalIndex, int version) {
        if (version >= 0 && version < 5 && originalIndex >= 0 && originalIndex < 5) {
            return SHUFFLE[originalIndex][version];
        }
        return originalIndex;
    }

    public static int[][] getShuffleMatrix() {
        return SHUFFLE.clone();
    }
}