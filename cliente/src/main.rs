slint::include_modules!();
use std::io::{Read, Write};
use std::net::{TcpStream};
use std::error::Error;
use std::ops::Index;
use std::io::{BufRead};
use std::str::{from_utf8, from_utf8_mut};
use std::sync::mpsc;
use std::thread;
use std::time::Duration;
use bytes::Bytes;
use json::{JsonValue, value};


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

fn main() -> Result<(), slint::PlatformError> {
    let ui = AppWindow::new()?;
    let ui_clone=ui.as_weak();
    let (tx, rx) = mpsc::channel();

    let _tcpthread=
        thread::Builder::new().name("threadtcp".to_string()).spawn(move||{

        loop {
            thread::sleep(Duration::from_millis(2000));
            match TcpStream::connect("localhost:8085") {
                Ok(mut stream) => {
                    println!("Successfully connected to server in port 8085");

                    let parsed = json::parse(r#"
                    {
                        "command": "Get playlist"
                    }
                    "#).unwrap();
                    let mensaje=parsed.to_string();
                    let msg=mensaje.as_bytes();

                    stream.write(&*msg).unwrap();
                    println!("Sent Json, awaiting reply...");

                    let mut buf = [0;512];

                    let stmclone = stream.try_clone();
                    match stmclone.unwrap().read(&mut buf){
                        Ok(n) => {
                            let mhh = from_utf8_mut(&mut buf).unwrap().to_string();
                            let my_string= mhh.trim_matches(char::from(0));
                            let myjson= json::parse(my_string).unwrap();
                            println!("{}", myjson["status"]);
                            println!("Received {:?} bytes", n);
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
            tx.send("connected").unwrap();
        }
    }).expect("TODO: panic message");

    let _ui_edit_thread=
        thread::Builder::new().name("ui_edit_thread".to_string()).spawn({move|| {
            for recived in rx{
                //https://docs.rs/i-slint-core/1.5.0/i_slint_core/api/struct.Weak.html#method.upgrade_in_event_loop
                ui_clone.upgrade_in_event_loop(move |ui| ui.set_estado(recived.into())).expect("Changed");
                ui_clone.upgrade_in_event_loop(move |ui| ui.set_visibilidad(false.into())).expect("Changed");
            }
    }}).expect("TODO: panic message");

    ui.run()
}
