```mermaid
flowchart TD
    A[Start enter_dir] --> B{count is zero}
    B -->|yes| Z[return]
    B -->|no| C[Take selected item by cursor]
    C --> D{is directory}
    D -->|no| Z
    D -->|yes| E{name equals parent}
    E -->|yes| F[call up_dir]
    E -->|no| G{name equals dot}
    G -->|yes| H[keep path]
    G -->|no| I[join current path and name]
    I --> J[write new path]
    H --> K[reset cursor and scroll]
    F --> K
    J --> K
    K --> L[load_dir]
    L --> Z
```

## 3.2 Функциональная схема: `load_dir`
```mermaid
flowchart TD
    A[Start load_dir] --> B[opendir current path]
    B -->|ошибка| Z[return]
    B -->|ok| C[t.count = 0]
    C --> D{readdir && count < MAX_FILES}
    D -->|yes| E[Заполнить f->name, selected]
    E --> F[join full path]
    F --> G{lstat ok}
    G -->|yes| H[Заполнить is_dir/is_link/size/mode/mtime]
    G -->|no| I[Поставить поля в 0]
    H --> J[count++]
    I --> J
    J --> D
    D -->|no| K[Нормализовать cursor]
    K --> L[sort_tab]
    L --> M[closedir]
    M --> N[return]
```

## 3.3 Диаграмма состояний
```mermaid
stateDiagram-v2
    [*] --> BrowsingDirectory

    BrowsingDirectory --> PromptingUser: F7 (new dir)
    PromptingUser --> BrowsingDirectory: submit/cancel

    BrowsingDirectory --> PromptingUser: F4 (rename)
    PromptingUser --> BrowsingDirectory: submit/cancel

    BrowsingDirectory --> ConfirmingDelete: F8
    ConfirmingDelete --> BrowsingDirectory: y/n

    BrowsingDirectory --> BrowsingDirectory: Up/Down/Left/Right/Enter/Backspace/F5/F6/TAB/t/w
    BrowsingDirectory --> Terminated: q
    Terminated --> [*]
```

## 3.4 Диаграмма взаимодействий (пример: F5 copy)
```mermaid
sequenceDiagram
    actor User
    participant UI as ui.c (draw/getch)
    participant IN as input.c handle_input
    participant FS as fs.c copy_file
    participant PN as panel.c load_dir

    User->>UI: Нажимает F5
    UI->>IN: handle_input(KEY_F5, ...)
    IN->>FS: copy_file(src, dst)
    FS-->>IN: ok
    IN->>PN: load_dir(target_tab)
    PN-->>IN: target updated
    IN-->>UI: return 0
    UI->>UI: draw_ui(...)
```
