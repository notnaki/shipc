#struct Person {
#    Name: string;
#    Age: int;
#    Height: int;
#}
#
fn main() void {

    let x:string = f"{true} | {false}";
    
    printf(f"{x}\n");
    x = "hey";

    let f:[1][10]int ={{1,2,3,4,5,6,7,8,9,0}};
    f[0][8] = 10009 * 200 * 9 / 19813 + 5;
    printf(f"{f[0][8]}");
}