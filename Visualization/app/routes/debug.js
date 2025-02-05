import express from 'express'
import fs from 'fs'
import path from 'path'
import { fileURLToPath } from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);
const router = express.Router();

router.get("/", (req, res) => {
    res.sendFile(path.join(__dirname, "../public/debug/index.html"));
});

router.get('/list', (req, res) => {
    fs.readdir(path.join(__dirname, "../public/debug"), (err, files) => {
        if (err) return res.status(500).json({ error: 'Failed to read directory' });
        res.json(files.filter(file => file !== 'index.html'));
    });
});

export default router;
