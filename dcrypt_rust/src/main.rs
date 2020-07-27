use dcrypt::Dcrypt;

fn main(){
    let dcrypt: Dcrypt = Dcrypt::new();
    let input: Vec<u8> = vec![1,2,3,4,5,6,7,8,9,10];
    println!("Input    : {:?}", input);
    match dcrypt.encrypt(&input) {
        Ok(v) => {
            println!("Encrypted: {:?}", v);
            match dcrypt.decrypt(&v) {
                Ok(v1) => println!("Decrypted: {:?}", v1),
                Err(e) => println!("Failed to decrypt! {}", e.message)
            }
        },
        Err(e) => println!("Failed to encrypt! {}", e.message)
    }
}