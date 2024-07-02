struct Hey{
    i :int;
}

fn main() int {

    let x:[1]Hey = { new Hey{10} };

    printf("%i", x[0].i);
    #printf("%i",x.i);

    return 0;
}


