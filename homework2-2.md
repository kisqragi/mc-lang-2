# TODO 2.1
```
if (isalpha(lastChar)) {
    std::string alphaStr = "";
    alphaStr += lastChar;
    while (isalpha(lastChar = getNextChar(iFile))) {
        alphaStr += lastChar;
    }
    setIdentifier(alphaStr);
    if (alphaStr == "def") {
        return tok_def;
    }
    return tok_identifier;
}
```
基本的にはTODO 1.3と同じ実装で行いました。  
2.1はアルファベットの判定なので`isalpha`を使いました。


# TODO 2.2 and 2.3
ここで少しはまっていました。  
## 前提
まず今回の関数の定義、呼び出しの前提について自分なりに理解したところをメモします。  
### <u>関数の定義について</u>
```
def foo(x y z)
    x + y + z;
```
この式を例にします。  
関数の定義としては簡単に言えばdefから始まって次に識別子(今回はfoo)があれば定義になります。  
そして関数の引数について、定義のときは上記のようにスペースで区切っています。個人的にここが意外と大事でした。  
普段の引数が(int x, int y)のような形で','ありに慣れていたので少し戸惑いました。  
そして関数の本体(body)はスコープという概念はなく、定義式の次の式がbodyになります。  
実装する側になって気づいたのですが、関数の中に複数の式を設定できるスコープは便利だなあ、と思いました。  

### <u>関数の呼び出し</u>
```
foo (x, y, z);
```
呼び出し側はこんな感じです。  
違う点といえばまず、定義ではないのでdefがないです。  
これによってtok_defを返すかdef_identifierを返すかで定義か呼び出しかを判定しています。  
そしてここが自分の躓きポイントだったのですが、呼び出しの場合は引数は','区切りで指定します。  
  

### <u>実装</u>


最初の実装を以下のようにしてしまっていたので細かな部分でエラーが出ました。  
```
while (CurTok != ')') {
    auto result = ParseExpression();
    args.push_back(std::move(result));
    if (CurTok != ',') {
        return LogError("expected ')' or ',' in argument list");
    }
    getNextToken();
}
```
これだと引数がない場合の呼び出しは良いのですが、引数がある場合にはエラーが出てしまいます。  
なのでこれを修正しました。  
```
if (CurTok != ')') {
    while (1) {
        auto result = ParseExpression();
        args.push_back(std::move(result));
        if (CurTok == ')') {
            break;
        }
        if (CurTok != ',') {
            return LogError("expected ')' or ',' in argument list");
        }
        getNextToken();
    }
}
```
これで')'があればループを抜け、')'でなく','でもなければエラーを返す処理ができました。

### <u>TODO 2.3</u>
実装に関してはコメントの通りほぼ2.2と同じでした。  
ですが、最初前提の部分でも書いたのですが、区切りが','ではなく、getNextToken()を呼ぶだけという部分に少し引っかかっていましたが、前提を考えてみたら無駄な時間だったと感じました。  

### <u>TODO 2.2 と 2.3の大まかな流れ</u>
ここは今もかなり曖昧なので違う点が多々あるかもしれませんが、間違いがあれば指摘お願いします。  

TODO 2.2  
識別子のパース  
```
MainLoop  
    ↓  
    HandleTopLevelExpression
        ↓  
        ParseTopLevelExpr
            ↓
            (1)ParseExpression
                ↓
                (2)ParsePrimary
                    ↓
                    (3)ParseIdentifier
引数がある場合1,2,3をループする。
(1) → (2) → (3) → (1) → (2) → (3)
```
1,2,3をループすることで引数名をVariableASTのメンバとしてargsにpushします。  

TODO 2.3  
シグネチャの解析  
```
MainLoop  
    ↓  
    HandleDefinition  
        ↓  
        ParseDefinition
            ↓
            ParsePrototype  ---> proto
            ParseExpression ---> body

            proto + body = FunctionAST
```
ParsePrototypeが処理されると、FunctionASTのprotoメンバに関数名と引数名を格納。ParseExpressionで関数のbody(x + y + zなど)を格納する。  
<br>
こんな流れかな、と思います。  

# TODO 2.4
実装コード
```
Value *V = NamedValues[variableName];
return V ? V : LogErrorV("unknown error name");
```
実装自体はこういう書き方ができるというのを調べて実装しました。  
しかし、NamedValuesというのは使った記憶がなく、いつ引数名と値を格納しているのかなと思って少し調べてみました。  
自分が思いついたあたりはcodegen.hの118行あたりの
```
for (auto &Arg : function->args())
    NamedValues[Arg.getName()] = &Arg;
```
この式がmapにkeyとvalueをsetしているのではないかと思いました。
<br>

# TODO 2.5
ここはコメント通りに実装しただけです。

# TODO 2.6
自身だけの力ではありませんが、自作したコンパイラがきちんと関数の定義、呼び出しをきちんと処理してくれていて感動しました！
　　