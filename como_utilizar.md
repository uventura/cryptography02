# Cryptography02

## Como utilizar?

Atualmente o projeto está disponível apenas em plataforma Linux, para utilizar apropriamente será necessário os seguintes comandos em um ambiente como Ubuntu

```shell
`
sudo apt-get install -y libgmp-dev
sudo apt-get install -y libcrypto++-dev

```

E para realizar os testes:

```shell
`
./tools/bazel test --test_output=all ...
```

> Não é necessário rodar o programa localmente, o projeto está disponível em https://github.com/uventura/cryptography02, e você pode verificar os resultados dos testes na tab de *actions* que o GitHub.

## Como verificar os testes no GitHub

Em https://github.com/uventura/cryptography02, vá na tab **actions** e então clique no primeiro elemento da lista exibida e por fim, no bloco de teste realizado, exemplo:

(verifique a pasta de imagens)

`<img src = "images/01_actions.png"/>`

`<img src = "images/02_testing.png"/>`

`<img src = "images/03_verify_tests.png"/>`
