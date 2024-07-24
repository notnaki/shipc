struct Person {
    name: string;
    age: int;
}

impl Person {
    fn Hello(self:&Person, message:string) void {
        printf(f"Hello, {self.name} {message}!");
    }
}

fn main() void {
    let p:&Person = new Person {"John",  42 };
    p.Hello("Hey");

}
