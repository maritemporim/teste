import java.util.Scanner;

public class Ls {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        String palavra;
        palavra = sc.nextLine();

        while(isFim(palavra) == false){
            String vogal = isVogal(palavra);
            String consoante = isConsoante(palavra);
            String inteiro = isInteiro(palavra);
            String real = isReal(palavra); 

            System.out.println(vogal + " " + consoante + " " + inteiro + " " + real);
            palavra = sc.nextLine();
        }
        sc.close();
    }

    public static String isConsoante(String palavra){
        String resp = "SIM";
        for(int i = 0; i < palavra.length(); i++){
            if(palavra.charAt(i) == 'a' || palavra.charAt(i) == 'e' || palavra.charAt(i) == 'i' || palavra.charAt(i) == 'o' || palavra.charAt(i) == 'u' || palavra.charAt(i) < '0' || palavra.charAt(i) > '9'){
            resp = "NAO";
            }
        }
        return resp;
    }

    public static String isVogal(String palavra){
        String resp = "SIM";
        for(int i = 0; i < palavra.length(); i++){
            if(palavra.charAt(i) != 'a' || palavra.charAt(i) != 'e' || palavra.charAt(i) != 'i' || palavra.charAt(i) != 'o' || palavra.charAt(i) != 'u'){
            resp = "NAO";
            }
        }
        return resp;
    }

    public static String isReal(String palavra) {
    int pontos = 0;
    int digitos = 0;

    for (int i = 0; i < palavra.length(); i++) {
        char c = palavra.charAt(i);

        if (c == '.' || c == ',') {
            pontos++;
        } 
        else if (c >= '0' && c <= '9') {
            digitos++;
        } 
        else {
            return "NAO";
        }
    }

    if (digitos > 0 && pontos <= 1) {
        return "SIM";
    }

    return "NAO";
}

    public static String isInteiro(String palavra){
        String resp = "SIM";
        for(int i = 0; i < palavra.length(); i++){
            if (palavra.charAt(i) < '0' || palavra.charAt(i) > '9') {
            resp = "NAO";
}
        }
        return resp;
    }

    public static boolean isFim(String palavra){
        boolean resp = false;
        if(palavra.length() == 3 && palavra.charAt(0) == 'F' && palavra.charAt(1) == 'I' && palavra.charAt(2) == 'M'){
            resp = true;
        }
        return resp;
    }
}
