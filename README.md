# 42_pipex

pipexはパイプやリダイレクトをプログラムで実装する課題。

## 実行方法
```
make
./pipex infile cmd1 cmd2 outfile
```
で以下と同等の出力結果が得られる。
```
<infile cmd1 | cmd2 >outfile
```
＊引数は４つで固定。順番の入れ替えは不可。

また以下を実行すると、マルチパイプ、ヒアドキュメントを実行することができる。
```
make bonus
```
### マルチパイプ
```
./pipex infile cmd1 cmd2 cmd3 cmd4 cmd5 ..... outfile
```
は以下と同等に実行される。
```
<infile cmd1 | cmd2 | cmd3 | cmd4 | cmd5 .... >outfile
```

### ヒアドキュメント
```
./pipex here_doc EOF cmd1 cmd2 outfile
```
は以下と同等に実行される。
```
cat << EOF | grep a >> outfile
```
＊引数は５つで固定。順番の入れ替えは不可

## 解説資料
https://www.notion.so/Pipex-73e0cd1913954e5a9dc729967fee4624

