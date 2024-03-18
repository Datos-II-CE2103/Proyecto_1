slint::include_modules!();

fn main() -> Result<(), slint::PlatformError> {
    let ui = AppWindow::new()?;

    ui.set_estado("disconnected".into());
    ui.on_request_increase_value({
        let ui_handle = ui.as_weak();
        move || {
            println!("btn pressed");
            let ui = ui_handle.unwrap();
            ui.set_estado("connected".into());
        }
    });

    ui.run()
}
