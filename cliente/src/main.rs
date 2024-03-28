slint::include_modules!();
use std::io::{Read, Write};
use std::net::{Shutdown, TcpListener, TcpStream};
use std::net::{SocketAddr};
use std::os::unix::raw::uid_t;
use std::str::from_utf8;

use std::thread;

macro_rules! attempt { // `try` is a reserved keyword
   (@recurse ($a:expr) { } catch ($e:ident) $b:block) => {
      if let Err ($e) = $a $b
   };
   (@recurse ($a:expr) { $e:expr; $($tail:tt)* } $($handler:tt)*) => {
      attempt!{@recurse ($a.and_then (|_| $e)) { $($tail)* } $($handler)*}
   };
   ({ $e:expr; $($tail:tt)* } $($handler:tt)*) => {
      attempt!{@recurse ($e) { $($tail)* } $($handler)* }
   };
}

fn listenertcp() -> std::io::Result<()>{
    println!("listcp");
    let listener = TcpListener::bind("127.0.0.1:8929")?;
    // accept connections and process them serially
    for stream in listener.incoming() {
        handle_client(stream?);
    }
    Ok(())
}

fn handle_client(stream: TcpStream) {
    println!("conexion entrante")
}

fn main() -> Result<(), slint::PlatformError> {
    let ui = AppWindow::new()?;

    thread::Builder::new().name("thread1".to_string()).spawn(|| {
        println!("prethread");
        listenertcp().expect("TODO: panic message");
        println!("connected");
    }).expect("TODO: panic message");

    ui.set_estado("na".into());

    ui.on_request_increase_value({
        let ui_handle = ui.as_weak();
        move || {
            println!("btn pressed");

            thread::Builder::new().name("thread1".to_string()).spawn(|| {
                match TcpStream::connect("192.168.100.11:8085") {
                    Ok(mut stream) => {
                        println!("Successfully connected to server in port 8085");

                        let msg = b"Hello!";

                        stream.write(msg).unwrap();
                        println!("Sent Hello, awaiting reply...");

                        let mut data = [0 as u8; 6]; // using 6 byte buffer
                        match stream.read_exact(&mut data) {
                            Ok(_) => {
                                if &data == msg {
                                    println!("Reply is ok!");
                                } else {
                                    let text = from_utf8(&data).unwrap();
                                    println!("Unexpected reply: {}", text);
                                }
                            },
                            Err(e) => {
                                println!("Failed to receive data: {}", e);
                            }
                        }
                    },
                    Err(e) => {
                        println!("Failed to connect: {}", e);
                    }
                }
                println!("Terminated.");

            }).expect("TODO: panic message");

            let ui = ui_handle.unwrap();
            ui.set_estado("connected".into());
        }
    });
    ui.run()
}
