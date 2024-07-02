struct Hey{
    M_field : string;
    M_f : int;
}


fn main() int {
    let x:Hey = new Hey{"hey", 20};
    printf("%s",x.M_field);

    return 0;
}


