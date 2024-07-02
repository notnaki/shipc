
fn a() int {
    let x: [1]int = {20};
    return x[0];
}

fn main() int {
    let f:int = a();
    let x:int = 10;
    printf("%i",x+f);
    return x+f;
}


