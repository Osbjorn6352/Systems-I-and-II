class PlusMinus {

    /*
     * Complete the 'plusMinus' function below.
     *
     * The function accepts INTEGER_ARRAY arr as parameter.
     */

    public static void plusMinus(List<Integer> arr) {
    // Write your code here
        float posCount = 0; //declaring counters for each value possibility
        float negCount = 0;
        float zerCount = 0;
        for (int i = 0; i < arr.size(); i++){ //increment counts according to value type
            if (arr.get(i) > 0) posCount += 1;
            if (arr.get(i) < 0) negCount += 1;
            if (arr.get(i) == 0) zerCount += 1;
        } 
        //Next, perform division for each count to get ratio
        System.out.format("%.6f\n", posCount/arr.size()); //format to six decimals
        System.out.format("%.6f\n", negCount/arr.size()); 
        System.out.format("%.6f\n", zerCount/arr.size());
    }

}
