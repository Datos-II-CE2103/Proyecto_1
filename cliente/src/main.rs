slint::include_modules!();
use std::io::{Read, Write};
use std::net::{TcpListener, TcpStream};
use std::error::Error;
use std::ops::Index;
use std::os::unix::raw::gid_t;
use std::str::from_utf8;
use std::sync::mpsc;
use std::thread;
use std::time::Duration;
use futures::AsyncReadExt;
use signals2::*;
use slint::Weak;

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
            let mut respuestaJson =String::new();
            let mut buffer = String::new();
            thread::sleep(Duration::from_millis(5000));
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
                    match stream.read_to_string(&mut buffer) {
                        Ok(_) => {
                            respuestaJson=buffer;
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
            let mut respuesta=json::from(respuestaJson);
            if respuesta["status"] == "ok" {
                println!("Reply is ok!");
            } else {
                println!("Unexpected reply: {}", respuesta["status"]);
            }
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
