<p align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="assets/logo-dark.svg">
    <img alt="Marginália" src="assets/logo-light.svg" width="390">
  </picture>
</p>

---

O Marginália é usado no balcão da livraria para cadastrar autores e livros, montar o pedido de cada cliente e enviar o pagamento ao gateway. O software lê um comando por linha da entrada padrão e responde a cada um com uma única linha. O código-fonte é escrito em C++17, usa apenas a biblioteca padrão e mantém uma classe por entidade do domínio: `Author`, `Book`, `Customer` e `Invoice`.

## Construção

```sh
git clone <endereço do repositório>
cd marginalia
make
```

O comando `make` gera o executável `build/marginalia` e o executável dos testes, `build/tests`, e exige apenas o `g++` com suporte a C++17 e o `make`.

## Uso

```sh
build/marginalia <<'EOF'
customer 7 10 Ana Souza
author 1 F J. K. Rowling
book 1 1 49.90 Harry Potter e a Pedra Filosofal
book 2 1 54.90 Harry Potter e a Câmara Secreta
add 1 2
add 2 1
invoice
pay
EOF
```

```
Invoice opened for customer 7
Author 1 registered
Book 1 registered
Book 2 registered
Book 1 added to the invoice with quantity 2
Book 2 added to the invoice with quantity 1
Invoice[id = 1, Customer[id = 7, name = Ana Souza, discount = 10], total = 139.23]
Payment of 139.23 authorized
```

| Comando | Descrição |
|---|---|
| `customer <id> <discount> <name>` | Abre o pedido do cliente |
| `author <id> <M or F> <name>` | Cadastra um autor |
| `book <id> <author_id> <price> <title>` | Cadastra um livro |
| `add <book_id> <quantity>` | Adiciona um livro ao pedido |
| `remove <book_id>` | Remove um livro do pedido |
| `invoice` | Mostra o pedido, com o total já descontado |
| `pay` | Paga o total do pedido |

As linhas vazias e as linhas que começam com `#` são ignoradas, e as respostas de erro começam sempre com `Error`.

## Regras

| Código | Regra |
|---|---|
| RN-01 | O nome do autor é armazenado com no máximo 47 bytes, e o gênero assume apenas os valores M ou F. |
| RN-02 | O título do livro é armazenado com no máximo 63 bytes, e o preço corresponde a um valor não negativo. |
| RN-03 | O desconto do cliente corresponde a um percentual inteiro, compreendido entre 0 e 100, inclusive. |
| RN-04 | A quantidade de cada livro incluído no pedido corresponde a um número inteiro estritamente positivo. |
| RN-05 | O pedido comporta no máximo 10 livros distintos, e cada um deles figura no pedido uma única vez. |
| RN-06 | As credenciais de serviços externos obedecem às boas práticas de gestão e de proteção de segredos. |

## Testes

```sh
build/tests            # executa os testes de unidade
build/tests --list     # lista os testes
```

## Verificações

```sh
make sast              # SAST, revisão de software automatizada com o Semgrep (pipx install semgrep==1.182.1)
make sanitizers        # testes de software compilados com ASan e UBSan
make valgrind          # testes de software executados sob o Valgrind (sudo apt-get update && sudo apt-get install -y valgrind)
make dast              # DAST, testes de software com casos hostis executados contra build/marginalia
```

> [!NOTE]
> Em kernels Linux recentes, inclusive nos runners do GitHub Actions, o ASan pode falhar ao iniciar com a mensagem `AddressSanitizer:DEADLYSIGNAL`. Nesse caso, execute `sudo sysctl -w vm.mmap_rnd_bits=28` antes de `make sanitizers`.

## Utilitários

```sh
make new-key           # gera uma chave falsa do gateway de pagamentos
```

## Pipeline

O pipeline de integração contínua, definido em [`.github/workflows/ci.yml`](.github/workflows/ci.yml), é executado no GitHub Actions a cada push na `develop` e nas branches `feature/**`, e a cada pull request para a `develop`.

## GitFlow

O repositório segue o GitFlow: a `master` guarda as versões entregues, a `develop` integra a próxima versão, e cada mudança nasce numa branch `feature/*`, integrada à `develop` por pull request.

## Estrutura

```
marginalia/
├── .github/workflows/ci.yml   # pipeline de integração contínua
├── .semgrep/cpp-rules.yml     # regras do SAST
├── assets/                    # logotipo nas versões clara e escura
├── src/
│   ├── include/               # cabeçalhos das classes e do pagamento
│   ├── author.cpp
│   ├── book.cpp
│   ├── customer.cpp
│   ├── invoice.cpp
│   ├── main.cpp               # interpretação dos comandos
│   └── payment.cpp            # integração com o gateway de pagamentos
├── tests/
│   ├── dast/hostile_cases.cpp # casos hostis do DAST
│   └── unit/tests.cpp         # testes de unidade
├── tools/new_key.cpp          # gerador de chave falsa
└── Makefile
```
