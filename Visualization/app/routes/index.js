import express from 'express'
import path from 'path'

const __dirname = import.meta.dir;
const __filename = import.meta.path;
const router = express.Router();

router.get("/", (req, res) => {
    res.sendFile(path.join(__dirname, "../public/index.html"));
});

export default router;
