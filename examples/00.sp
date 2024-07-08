
fn isPrimt(n:int) bool {
    if (n <= 1){
        return false;
    }
    if (n == 2){
        return true;
    }

    if (n % 2 == 0){
        return false;
    }

    let i:int = 3;

    while (i < n){
        if (n % i == 0){
            return false;
        }
        i = i+2;
    }

    return true;
}

fn findPrimes(maxNumber:int) void {
    printf("Prime numbers up to %i\n",maxNumber );

    let i:int = 2;
    while (i <= maxNumber){
        if (isPrimt(i)){
            printf("is prime %i\n", i);
        }
        i = i+1;
    }
}

fn main() void {
    
    

    findPrimes(1000000);


}


