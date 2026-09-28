import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.Locale;
import java.util.Scanner;

class Data {
    private int ano;
    private int mes;
    private int dia;

    public Data(int ano, int mes, int dia) {
        this.ano = ano;
        this.mes = mes;
        this.dia = dia;
    }

    public int getAno() { return ano; }
    public int getMes() { return mes; }
    public int getDia() { return dia; }

    // parse de uma string no formato aaaa-mm-dd
    public static Data parseData(String s) {
        Data data = null;
        if (s != null && !s.isEmpty()) {
            String[] partes = s.split("-");
            if (partes.length == 3) {
                int ano = Integer.parseInt(partes[0].trim());
                int mes = Integer.parseInt(partes[1].trim());
                int dia = Integer.parseInt(partes[2].trim());
                data = new Data(ano, mes, dia);
            }
        }
        return data;
    }

    // dd/mm/yyyy
    public String format() {
        return String.format("%02d/%02d/%d", dia, mes, ano);
    }
}

class Veiculo {
    private int id;
    private String marca;
    private String modelo;
    private int ano;
    private String categoria;
    private String[] combustivel;
    private int cilindros;
    private double cilindrada;
    private String transmissao;
    private String tracao;
    private double consumoCidade;
    private double consumoEstrada;
    private double co2;
    private boolean turbo;
    private Data dataRegistro;

    public Veiculo(int id, String marca, String modelo, int ano, String categoria, String[] combustivel,
                   int cilindros, double cilindrada, String transmissao, String tracao,
                   double consumoCidade, double consumoEstrada, double co2, boolean turbo, Data dataRegistro) {
        this.id = id;
        this.marca = marca;
        this.modelo = modelo;
        this.ano = ano;
        this.categoria = categoria;
        this.combustivel = combustivel;
        this.cilindros = cilindros;
        this.cilindrada = cilindrada;
        this.transmissao = transmissao;
        this.tracao = tracao;
        this.consumoCidade = consumoCidade;
        this.consumoEstrada = consumoEstrada;
        this.co2 = co2;
        this.turbo = turbo;
        this.dataRegistro = dataRegistro;
    }

    public int getId() { return id; }
    public String getMarca() { return marca; }
    public String getModelo() { return modelo; }
    public int getAno() { return ano; }
    public String getCategoria() { return categoria; }
    public String[] getCombustivel() { return combustivel; }
    public int getCilindros() { return cilindros; }
    public double getCilindrada() { return cilindrada; }
    public String getTransmissao() { return transmissao; }
    public String getTracao() { return tracao; }
    public double getConsumoCidade() { return consumoCidade; }
    public double getConsumoEstrada() { return consumoEstrada; }
    public double getCo2() { return co2; }
    public boolean isTurbo() { return turbo; }
    public Data getDataRegistro() { return dataRegistro; }

    public static Veiculo parseVeiculo(String s) {
        if (s == null || s.isEmpty()) return null;
        String[] campos = s.split(",");

        int id = Integer.parseInt(campos[0].trim());
        String marca = campos[1].trim();
        String modelo = campos[2].trim();
        int ano = Integer.parseInt(campos[3].trim());
        String categoria = campos[4].trim();

        String[] combustivel = campos[5].trim().split(";");
        for (int i = 0; i < combustivel.length; i++) {
            combustivel[i] = combustivel[i].trim();
        }

        int cilindros = Integer.parseInt(campos[6].trim());
        double cilindrada = Double.parseDouble(campos[7].trim());
        String transmissao = campos[8].trim();
        String tracao = campos[9].trim();
        double consumoCidade = Double.parseDouble(campos[10].trim());
        double consumoEstrada = Double.parseDouble(campos[11].trim());
        double co2 = Double.parseDouble(campos[12].trim());
        boolean turbo = Boolean.parseBoolean(campos[13].trim());
        Data dataRegistro = Data.parseData(campos[14].trim());

        return new Veiculo(id, marca, modelo, ano, categoria, combustivel, cilindros,
                           cilindrada, transmissao, tracao, consumoCidade, consumoEstrada,
                           co2, turbo, dataRegistro);
    }

    public String format() {
        StringBuilder sbComb = new StringBuilder();
        for (int i = 0; i < combustivel.length; i++) {
            sbComb.append(combustivel[i]);
            if (i < combustivel.length - 1) {
                sbComb.append(",");
            }
        }

        String dataStr = (dataRegistro != null) ? dataRegistro.format() : "";

        return String.format(Locale.US,
                "[%d ## %s ## %s ## %d ## %s ## [%s] ## %d ## %.1f ## %s ## %s ## %.2f ## %.2f ## %.1f ## %b ## %s]",
                id, marca, modelo, ano, categoria, sbComb.toString(), cilindros,
                cilindrada, transmissao, tracao, consumoCidade, consumoEstrada,
                co2, turbo, dataStr);
    }
}

class LeitorCsv {
    public static Veiculo[] ler(String caminhoArquivo) {
        int totalLinhas = 0;

        try (BufferedReader br = new BufferedReader(new FileReader(caminhoArquivo))) {
            br.readLine(); // cabecalho
            String linha;
            while ((linha = br.readLine()) != null) {
                if (!linha.trim().isEmpty()) {
                    totalLinhas++;
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        Veiculo[] veiculos = new Veiculo[totalLinhas];
        int idx = 0;

        try (BufferedReader br = new BufferedReader(new FileReader(caminhoArquivo))) {
            br.readLine(); // cabecalho
            String linha;
            while ((linha = br.readLine()) != null) {
                if (!linha.trim().isEmpty()) {
                    Veiculo v = Veiculo.parseVeiculo(linha);
                    if (v != null) {
                        veiculos[idx++] = v;
                    }
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        return veiculos;
    }
}

// celula da lista duplamente encadeada
class CelulaDupla {
    public Veiculo elemento;
    public CelulaDupla ant;
    public CelulaDupla prox;

    public CelulaDupla() {
        this(null);
    }

    public CelulaDupla(Veiculo elemento) {
        this.elemento = elemento;
        this.ant = null;
        this.prox = null;
    }
}

// lista duplamente encadeada com celula cabeca
class ListaDupla {
    private CelulaDupla primeiro;
    private CelulaDupla ultimo;
    private int n;

    public ListaDupla() {
        primeiro = new CelulaDupla();
        ultimo = primeiro;
        n = 0;
    }

    public void inserirInicio(Veiculo veiculo) {
        CelulaDupla tmp = new CelulaDupla(veiculo);
        tmp.ant = primeiro;
        tmp.prox = primeiro.prox;
        primeiro.prox = tmp;
        if (primeiro == ultimo) {
            ultimo = tmp;
        } else {
            tmp.prox.ant = tmp;
        }
        n++;
    }

    public void inserirFim(Veiculo veiculo) {
        CelulaDupla tmp = new CelulaDupla(veiculo);
        tmp.ant = ultimo;
        ultimo.prox = tmp;
        ultimo = tmp;
        n++;
    }

    public void inserir(Veiculo veiculo, int posicao) throws Exception {
        if (posicao < 0 || posicao > n) {
            throw new Exception("erro: posicao invalida!");
        }
        if (posicao == 0) {
            inserirInicio(veiculo);
        } else if (posicao == n) {
            inserirFim(veiculo);
        } else {
            CelulaDupla i = primeiro;
            for (int j = 0; j < posicao; j++) {
                i = i.prox; // ao final, i e a celula da posicao 
            }
            CelulaDupla tmp = new CelulaDupla(veiculo);
            tmp.ant = i;
            tmp.prox = i.prox;
            i.prox.ant = tmp;
            i.prox = tmp;
            n++;
        }
    }

    public Veiculo removerInicio() throws Exception {
        if (primeiro == ultimo) {
            throw new Exception("erro: lista vazia!");
        }
        CelulaDupla rem = primeiro.prox;
        primeiro.prox = rem.prox;
        if (rem == ultimo) {
            ultimo = primeiro;
        } else {
            rem.prox.ant = primeiro;
        }
        rem.prox = null;
        rem.ant = null;
        n--;
        return rem.elemento;
    }

    public Veiculo removerFim() throws Exception {
        if (primeiro == ultimo) {
            throw new Exception("erro: lista vazia!");
        }
        CelulaDupla rem = ultimo;
        ultimo = ultimo.ant;
        ultimo.prox = null;
        rem.ant = null;
        n--;
        return rem.elemento;
    }

    public Veiculo remover(int posicao) throws Exception {
        if (primeiro == ultimo || posicao < 0 || posicao >= n) {
            throw new Exception("erro: posicao invalida!");
        }
        if (posicao == 0) {
            return removerInicio();
        } else if (posicao == n - 1) {
            return removerFim();
        }
        CelulaDupla i = primeiro;
        for (int j = 0; j < posicao; j++) {
            i = i.prox; // i e a celula anterior a que sera removida
        }
        CelulaDupla rem = i.prox;
        i.prox = rem.prox;
        rem.prox.ant = i;
        rem.prox = null;
        rem.ant = null;
        n--;
        return rem.elemento;
    }

    public void mostrar() {
        for (CelulaDupla i = primeiro.prox; i != null; i = i.prox) {
            System.out.println(i.elemento.format());
        }
    }
}

public class Tp13 {
    public static void main(String[] args) {
        Veiculo[] todosVeiculos = LeitorCsv.ler("/tmp/veiculos.csv");

        Scanner scanner = new Scanner(System.in);
        ListaDupla lista = new ListaDupla();

        // primeira parte: ids ate -1, inseridos no fim da lista
        while (scanner.hasNext()) {
            String token = scanner.next();
            if (token.equals("-1")) {
                break;
            }
            Veiculo v = buscarVeiculo(todosVeiculos, Integer.parseInt(token));
            if (v != null) {
                lista.inserirFim(v);
            }
        }

        // segunda parte: n comandos
        if (scanner.hasNext()) {
            int nComandos = Integer.parseInt(scanner.next());
            for (int i = 0; i < nComandos && scanner.hasNext(); i++) {
                String cmd = scanner.next();
                try {
                    if (cmd.equals("II")) {
                        Veiculo v = buscarVeiculo(todosVeiculos, Integer.parseInt(scanner.next()));
                        if (v != null) lista.inserirInicio(v);
                    } else if (cmd.equals("IF")) {
                        Veiculo v = buscarVeiculo(todosVeiculos, Integer.parseInt(scanner.next()));
                        if (v != null) lista.inserirFim(v);
                    } else if (cmd.equals("I*")) {
                        int pos = Integer.parseInt(scanner.next());
                        Veiculo v = buscarVeiculo(todosVeiculos, Integer.parseInt(scanner.next()));
                        if (v != null) lista.inserir(v, pos);
                    } else if (cmd.equals("RI")) {
                        Veiculo v = lista.removerInicio();
                        System.out.println("(R)" + v.getMarca() + " " + v.getModelo());
                    } else if (cmd.equals("RF")) {
                        Veiculo v = lista.removerFim();
                        System.out.println("(R)" + v.getMarca() + " " + v.getModelo());
                    } else if (cmd.equals("R*")) {
                        int pos = Integer.parseInt(scanner.next());
                        Veiculo v = lista.remover(pos);
                        System.out.println("(R)" + v.getMarca() + " " + v.getModelo());
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
        scanner.close();

        lista.mostrar();
    }

    public static Veiculo buscarVeiculo(Veiculo[] veiculos, int id) {
        for (int i = 0; i < veiculos.length; i++) {
            if (veiculos[i].getId() == id) {
                return veiculos[i];
            }
        }
        return null;
    }
}