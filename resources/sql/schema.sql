CREATE TABLE IF NOT EXISTS contacts (
    id          INTEGER PRIMARY KEY AUTOINCREMENT,
    first_name  TEXT NOT NULL CHECK (length(trim(first_name)) > 0),
    last_name   TEXT,
    note        TEXT,
    created_at  TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS payments (
    id          INTEGER PRIMARY KEY AUTOINCREMENT,
    contact_id  INTEGER NOT NULL REFERENCES contacts(id) ON DELETE CASCADE,
    amount      INTEGER NOT NULL CHECK (amount > 0),
    type        INTEGER NOT NULL CHECK (type IN (0, 1, 2, 3)),
    date_time   TEXT NOT NULL,
    comment     TEXT
);

CREATE TABLE IF NOT EXISTS attachments (
    id           INTEGER PRIMARY KEY AUTOINCREMENT,
    payment_id   INTEGER NOT NULL REFERENCES payments(id) ON DELETE CASCADE,
    file_name    TEXT NOT NULL,
    stored_path  TEXT NOT NULL,
    added_at     TEXT NOT NULL
);

CREATE INDEX IF NOT EXISTS idx_payments_contact ON payments(contact_id);
CREATE INDEX IF NOT EXISTS idx_attachments_payment ON attachments(payment_id);