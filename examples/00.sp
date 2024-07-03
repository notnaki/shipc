struct Hey{
    s: string;
    i :int;
    h: Hey;
}

fn main() int {

    let f:Hey = new Hey{"b",20};
    let x: Hey = new Hey{"a",10, f};
    

    printf("%i, %s",x.i, x.h.s);

    return 0;
}


