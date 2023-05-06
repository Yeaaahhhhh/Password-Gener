import tkinter as tk
from tkinter import ttk
import secrets
import string
import os
from tkinter import messagebox
from tkinter import filedialog

def update_password_length_label(*args):
    password_length_label_var.set(str(password_length_var.get()))

def generate_passwords():
    try:
        password_length = int(password_length_var.get())
        password_count = int(password_count_var.get())
        use_uppercase = uppercase_var.get()
        use_lowercase = lowercase_var.get()
        use_digits = digits_var.get()
        use_symbols = symbols_var.get()
        open_text = open_text_var.get()

        if password_count < 1:
            raise ValueError("Password count should be a positive integer.")

        characters = ""

        if use_uppercase:
            characters += string.ascii_uppercase
        if use_lowercase:
            characters += string.ascii_lowercase
        if use_digits:
            characters += string.digits
        if use_symbols:
            characters += string.punctuation

        if not characters:
            raise ValueError("At least one character set must be selected.")

        passwords = []

        for _ in range(password_count):
            password = ''.join(secrets.choice(characters) for _ in range(password_length))
            passwords.append(password)

        output_file = "generated_passwords.txt"
        with open(output_file, 'w') as f:
            f.write("\n".join(passwords))

        if open_text:
            os.startfile(output_file)

    except ValueError as e:
        messagebox.showerror("Error", str(e))

root = tk.Tk()
root.title("Secure Password Generator")
root.geometry("700x400")
root.resizable(0, 0)

frame = ttk.Frame(root)
frame.place(relx=0.5, rely=0.5, anchor=tk.CENTER)

password_length_var = tk.IntVar(value=10)
password_length_slider = ttk.Scale(frame, from_=10, to=100, orient=tk.HORIZONTAL, variable=password_length_var, length=400, command=update_password_length_label)
password_length_slider.grid(row=0, column=1, padx=(0, 10), pady=10)
ttk.Label(frame, text="Password Length:").grid(row=0, column=0, sticky=tk.W, pady=10)

password_length_label_var = tk.StringVar(value=str(password_length_var.get()))
password_length_label = ttk.Label(frame, textvariable=password_length_label_var)
password_length_label.grid(row=0, column=2, sticky=tk.W, pady=10)

uppercase_var = tk.BooleanVar(value=True)
lowercase_var = tk.BooleanVar(value=True)
digits_var = tk.BooleanVar(value=True)
symbols_var = tk.BooleanVar(value=True)

ttk.Checkbutton(frame, text="Uppercase Letters", var=uppercase_var).grid(row=1, column=0, sticky=tk.W, pady=5)
ttk.Checkbutton(frame, text="Lowercase Letters", var=lowercase_var).grid(row=1, column=1, sticky=tk.W, pady=5)
ttk.Checkbutton(frame, text="Digits", var=digits_var).grid(row=2, column=0, sticky=tk.W, pady=5)
ttk.Checkbutton(frame, text="Symbols", var=symbols_var).grid(row=2, column=1, sticky=tk.W, pady=5)

password_count_var = tk.StringVar()
ttk.Label(frame, text="Number of Passwords:").grid(row=3, column=0, pady=10, sticky=tk.W)
ttk.Entry(frame, textvariable=password_count_var).grid(row=3, column=1, padx=(0, 10), pady=10, sticky=tk.W)

open_text_var = tk.BooleanVar(value=False)
ttk.Checkbutton(frame, text="Open Text File", var=open_text_var).grid(row=4, column=0, pady=5, sticky=tk.W)

generate_button = ttk.Button(frame, text="Generate", command=generate_passwords)
generate_button.grid(row=4, column=1, pady=5, sticky=tk.E)

root.mainloop()